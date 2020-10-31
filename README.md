# CommonSense PlatformIO Framework

This repository hosts the CommonSense firmware framework for platformIO, a VSCode/Atom plugin for embedded system development. 

The framework is intended to contain drivers and code that are useful across most projects. It contains the bare essentials.


## How do I use it?

Include the line

```
framework = commonsense
```

In your platform.ini file within the PIO project you want to run. The framework package should be downloaded from PIO servers and added directly to your installation. At that point, you should be able to use any and all header/C files within your project. 

Sometimes PIO does not index all the files within the framework, and it will think that the file is missing, designated by a squiggly red line beneath the name when it is included. You can verify if it is actually being tracked by ctl/cmd+click (depending on your platform) on the file, which should take you directly to that file. If it does not, then something is wrong. You can always delete the package by navigating to ```.platformio/packages``` and deleting the ```framework-commonsense``` directory. It will redownload next time you try to build the project. If the files are accessible but still show squiggles, try rebuilding the intellisense index and close+reopen the file you are working in.

## How do I update the framework, as a developer?

PlatformIO is now hosting packages for users, so this framework can be directly published with them. There appears to be a short waiting period, so the developers of this repo should be certain what they are publishing works as intended!


#### Below is supposed to work, but does not due to PIO errors! Skip to the next heading of this size for now (written 10/31/20)

To publish a package, first tarball the framework. Navigate into /frameworks/commonsense. Make sure to increment some part of the version number in ```package.json```, or else publishing this version will be ignored as a duplicate. Run ```pio package pack -o ../../published_packages``` to let PIO create this tarball itself (some in the team have noticed strangeness when trying to create the tarball manually from CLI). Use the -o option to specify where the tarball is stored in the repo. 

Next, we'll publish the package. Navigate to ```/published_packages/```. Publish by running ```pio package publish ./name-of-package.tar.gz```. If all is well, it will give a postive response, and probably indicate that it is under review. This seems to take the better part of a day, at the least. Hopefully this drops as we publish more often and gain some trust.


#### Stopgap publishing method

PIO seems to have some issues after a recent major version upgrade, so the package-publish flow is slightly different in the short term. Still run the ```pio package pack``` from ```/frameworks/commonsense```, but do not include a place for the file to be written. Let it stay here. Run ```pio package publish```. It should indicate the publish is under review. Move the file to the published directory, ```mv framework-commonsense-VERSION.tar.gz ../../published_packages``` to make sure it these archives don't snowball within later published versions. 

We will revisit this method in the future (early 2021) to have something more convenient. 

## How do I test the framework, as a developer?

Since publishing should only be done when the code is ready to rock, developers in this repo should do well to test their code before uploading. The legacy system required a manifest.json and publicly hosted link to the tarball, as opposed to publishing more centrally. We can still leverage parts of this for testing. 

To package up the framework in a realistic manner, use the SH script ```build_framework```, called with the version number as the only argument, as the following example shows:

```
./update_framework 1.0.1
```

This will set the package.json to the version number provided, ZIP the framework files (tarballs have had problems for undiagnosed reasons), and add some info to the manifest.json file (which we no longer need). It will version number each ZIP archive, and will copy the one just ZIPed into ```framework-commonsense-current.zip``` for version indepedent naming. Add, commit, and push this to the repo. 

To test this intermediate version naming, include the following line in the platformio.ini file:

```
platform_packages = framework-commonsense @ https://bitbucket.org/ccsg-res/commonsense-pio-framework/raw/master/framework-commonsense-current.zip
```

Note: if you are working from an alternative branch from master, change that part of the url to reference the name of the branch you're working from!

If the version number within is greater than the one previously installed, then it will download this package, overwriting the previous version in the local installation. You can now try running the framework however you like. 

When you are finished, removing the line for platform_packages will not revert us to the previous version, unfortunatley. Until a better method is found, navigate to where the packages are held within .platformio/packages, and delete the current framework. 

During development, it may be worth modifying files directly within the downloaded framework, and then copying the new code into where this repo is stored locally. This is inconvenient, and any alternative suggestions are welcome. 