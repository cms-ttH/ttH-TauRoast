MC Generation
=============

Pulling in basic steps from MCM
-------------------------------

We need the basic setup scripts to generate `LHE` inputs for the `GenSim`
step.  Go to the MCM page, select the `Requests` tab and click on
`Navigation` to search for the dataset to clone.  This should bring you to
a page like this one::

    https://cms-pdmv.cern.ch/mcm/requests?page=0&prepid=HIG-RunIISummer15wmLHEGS-00482

Now we can click the second icon from the left, which will download a setup
script like this:

.. literalinclude:: gs_setup.sh
   :language: shell
