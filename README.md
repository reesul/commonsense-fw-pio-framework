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

Navigate into /repo/path/frameworks/commonsense. Update the version of the framework within the 'package.json'

```
pio package pack -o ../../published_packages
cd ../../published_packages
pio package publish ./name-of-package.tar.gz
```

The version number will depend on what's in the package.json; make sure to publish that version. If all is well, it will give a postive response, and probably indicate that it is under review. This seems to take the better part of a day, at the least. Hopefully this drops as we publish more often and gain some trust.


#### Stopgap publishing method

PIO seems to have some issues after a recent major version upgrade, so the package-publish flow is slightly different in the short term. Still run the ```pio package pack``` from ```/frameworks/commonsense```, but do not include a place for the file to be written. Let it stay here. Run ```pio package publish```. It should indicate the publish is under review. Move the file to the published directory, ```mv framework-commonsense-VERSION.tar.gz ../../published_packages``` to make sure it these archives don't snowball within later published versions. 

We will revisit this method in the future (early 2021) to have something more convenient. 

## How do I test the framework, as a developer?

Since publishing should only be done when the code is ready to rock, developers in this repo should do well to test their code before uploading. It is recommended to do development on the version of the repo within .platformio/packages/framework-commonsense, and copy the changes into the repo. Once you're convinced it's working, it should be published.