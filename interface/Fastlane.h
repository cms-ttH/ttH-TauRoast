#ifndef __Fastlane_h
#define __Fastlane_h

// Components to process the slim event trees (with superslim components)
// faster than possible in python

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "TTree.h"

#include "SuperSlim.h"

struct _object;
typedef _object PyObject;

namespace fastlane {
   class BasicCut {
      public:
         BasicCut() {};
         BasicCut(const std::string& name) : name_(name) {};
         virtual ~BasicCut() {};

         virtual std::vector<std::string> processes() const = 0;

         const std::string& name() const { return name_; };
      private:
         std::string name_;
   };

   class Cut : public BasicCut {
      public:
         typedef bool (*fct_t)(
               const superslim::Event&,
               const std::vector<superslim::Tau>&,
               const std::vector<superslim::Tau>&,
               superslim::Tau::id,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Lepton>&,
               superslim::Lepton::id,
               const std::vector<superslim::Jet>&,
               const superslim::LorentzVector&);

         Cut() : BasicCut(), callback_(0) {};
         Cut(const std::string& name, fct_t eval) : BasicCut(name), fct_(eval), last_(std::make_tuple(-1, -1, -1)), callback_(0) {};
         virtual ~Cut() {};

         bool operator()(const std::string& process, const superslim::Event& e, const std::string& sys);

         void setCallback(PyObject* callback) { callback_ = callback; };

         virtual std::vector<std::string> processes() const override;
         virtual int& operator[](const std::string& process) { return counts_[process]; };
      private:
         typedef std::tuple<long, long, long> event_t;

         std::unordered_map<std::string, int> counts_;
         fct_t fct_;
         event_t last_;
         PyObject *callback_;
   };

   class StaticCut : public BasicCut {
      public:
         StaticCut() : BasicCut() {};
         StaticCut(const std::string& name) : BasicCut(name) {};
         virtual ~StaticCut() {};

         virtual std::vector<std::string> processes() const override;
         virtual double& operator[](const std::string& process) { return counts_[process]; };
      private:
         std::unordered_map<std::string, double> counts_;
   };

   class BasicLeaf {
      public:
         BasicLeaf() {};
         BasicLeaf(const std::string& name) : name_(name) {};
         virtual ~BasicLeaf() {};

         virtual void pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys) = 0;
         const std::string& name() const { return name_; };

         static std::vector<BasicLeaf*>& leaves() { return leaves_; };
         static void updateCache(const superslim::Event& e);
      protected:
         std::string name_;

         static std::vector<BasicLeaf*> leaves_;
         static std::vector<superslim::Lepton> cached_electrons_;
         static std::vector<superslim::Lepton> cached_muons_;
   };

   template<typename T>
   class Leaf : public BasicLeaf {
      public:
         typedef void (*fct_t)(
               const superslim::Event&,
               const std::vector<superslim::Tau>&,
               const std::vector<superslim::Tau>&,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Jet>&,
               const superslim::LorentzVector&,
               std::unordered_map<std::string, double>&,
               T& result);

         Leaf() : BasicLeaf(), val_(0) {};
         Leaf(const std::string& name, fct_t eval) : BasicLeaf(name), fct_(eval), val_(0) { leaves_.push_back(this); };
         virtual ~Leaf() {};

         void grow(TTree& t) { t.Branch(name_.c_str(), &val_); };
         virtual void pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys) override {
            val_ = T();
            fct_(e, e.taus(), e.allTaus(), e.leptons(), cached_electrons_, cached_muons_, e.allLeptons(), e.jets(sys), e.met(sys), w, val_);
         };

      private:
         fct_t fct_;
         T val_;
   };

   template<> void Leaf<std::vector<float>>::pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys);
   template<> void Leaf<std::vector<int>>::pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys);

   const TH1* get_cuts(const std::string& label, const std::vector<std::string>& files);
   void process(const std::string& process, const std::string& channel, const std::vector<std::string>& files, TTree& t, std::vector<fastlane::Cut*>& cuts, std::vector<fastlane::StaticCut*>& weights, const std::string& sys, PyObject* log, int max);
   void update_weights(std::unordered_map<std::string, double>& ws, const superslim::Event& e, const std::string& sys);
}

#endif
