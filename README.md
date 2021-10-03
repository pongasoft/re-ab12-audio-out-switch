A/B 12 Audio Out Switch (Rack extension)
========================================

This project contains the full source code for the free rack extension A/B 12 Audio Out Switch for Reason, the music DAW produced by Reason Studios. Check the [A/B 12 Audio Out Switch](https://pongasoft.com/rack-extensions/AB12AudioOutSwitch.html) website for more details.

### Requirements

* This project requires CMake (minimum version 3.17) properly installed (`cmake` executable must be in your `PATH`)
* This project currently expects RE SDK 4.2.0 or 4.1.0 to be installed on the machine (it will not download it for you)
* This project uses C++ 17 features
* Due to the RE SDK requirements, this project also requires python 3 to be installed
* It has been tested on macOS 10.14.6 with Xcode 9 installed
* It has been tested on Windows 10 with Visual Studio 16 2019 build tools

### Structure

* `Design` contains the Pixelmator files used to create the graphics
* `GUI2D` is a standard RE SDK folder which contains the images (png format) as well as `device_2D.lua` and `hdgui_2D.lua` files which defines the UI
* `Resources` is a standard RE SDK folder which contains the strings (English only) displayed in the UI
* `src/cpp` contains the C++ source code for the device
* `test/cpp` contains the C++ source code for the tests (using [re-mock](https://github.com/pongasoft/re-mock))
* `info.lua`, `motherboard_def.lua` and `realtime_controller.lua` are standard RE SDK files for defining the device
* `display.lua` is the standard SDK file which contains the lua code for the UI (various custom displays)
* `configure.py` is the python script you use to configure the CMake build

### Note about the RE SDK location

You can install the SDK wherever you want on your system and provide it as an argument to the `configure.py` script. Or you can install (or create a link) in a default location:

* `/Users/Shared/ReasonStudios/JukeboxSDK_<RE_SDK_VERSION>/SDK` for macOS
* `C:/Users/Public/Documents/ReasonStudios/JukeboxSDK_<RE_SDK_VERSION>/SDK` for Windows

Also note that the `RE2DRender` program needs to be unzipped and is expected to be a sibling of `SDK` (can be changed as well).

### Quick Starting guide

#### Step 1 - Configure the build

Invoking the `configure.py` **creates** a `build` directory in the directory where the command is run. Although it is strongly recommended to run this command outside the source tree, the `build` directory is excluded in `.gitignore` and since everything is contained within the `build` folder it is easy to clean after the fact.

Running the `python3 configure.py -h` command will print the usage.

> #### Note
> Depending on how `python` is installed on your system you may have to invoke it differently.

> #### Note
> This script is expecting the `cmake` command line tool to be in the path (use `cmake -version` to confirm it is properly installed).

If you have setup the RE SDK in its default location, as specified above, simply run `python3 configure.py` otherwise use the `-p` option to specify its location.

#### Step 2 - Install the plugin

After running the configure script, simply go into the newly generated `build` folder and run `re.sh install` (resp. `re.bat install`). This will build the plugin and install it in its default location.

> #### Note
> Simply run `re.sh -h` (resp. `re.bat -h`) for help with the command

> #### Note
> This script is expecting the `cmake` command line tool to be in the path (use `cmake -version` to confirm it is properly installed).

#### Step 3 - Try the plugin

You can then open Recon and load the newly built `A/B 12 Audio Out Switch` rack extension (you will find it under `Utilities | pongasoft | A/B 12 Audio Out Switch`)

> #### Info
> The plugin will **only** appear in Recon **not** Reason

### Changing the code, Debugging, etc...

If you want to change the code, run a debugger, etc... you can check the instructions for the [re-blank-plugin](https://github.com/pongasoft/re-blank-plugin#quick-starting-guide) which explain how to generate an XCode project, load it in an IDE, etc...

Release notes
-------------

#### 1.0.0 - 2021/10/03

* First release. Requires Reason 10.1+.

License
-------

- This project is released under the terms of the [Apache 2.0 license](LICENSE.txt)

- This project uses the Rack extension SDK released under the [Rack Extension License agreement](RE_License.txt)
