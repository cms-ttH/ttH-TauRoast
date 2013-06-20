#include <sstream>

#include "../interface/Helper.h"

using namespace std;

namespace roast {
   namespace Helper {
      vector<string> SplitString(const string &s) {
         vector<string> result;
         istringstream iss(s);
         while (iss) {
            string part;
            iss >> part;
            if (part != "")
               result.push_back(part);
         }
         return result;
      }
   }
}

NamespaceImp(roast)
