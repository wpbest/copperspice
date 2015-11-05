# copperspice
A Modern C++ Cross-platform Framework

Build CopperSpice and Samples on Windows 10 64-bit

Note: Windows does not provide a Bourne shell as a standard component. In order to build CopperSpice a shell program is required to run the configure scripts. This is installed as part of the MSYS project. In order to build CopperSpice or your applications, you will need to install MSYS and MinGW.

Software Tools

MSYS Version msys+7za+wget+svn+git+mercurial+cvs-rev9.7z

http://sourceforge.net/projects/mingwbuilds/files/external-binary-packages/

MSYS uses your Windows path and adds an entry for the MSYS bin folder. If you want to change your MSYS path, do not specify a drive letter in your path. The MSYS BASH utility treats the path where MSYS resides as the 'root'. As an example, if MSYS was installed to C:\Msys you need to add '/bin' to your path. Internally MSYS will expand this to C:\Msys\bin. The proper way to add or change your MSYS path is by altering the .bash_profile file. This file will normally be located in C:\msys\home\'username'.
Note: 'username' is the user you are logged into the windows operating system. i.e. wpbest is my user name in my case.
In the following line '/bin' refers to MSYS\bin folder.
export PATH=/bin:/c/mingw64/bin:/c/windows/system32

MinGW 64-bit Version x64-4.8.1-release-posix-seh-rev5.7z

http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.1/64-bit/threads-posix/seh/

MinGW, formerly called mingw32, is a development environment for Windows applications. It includes the GNU Compiler and other required tools. Since MinGW does not rely on any runtime DLL files, it is not necessary to distribute the source code with applications built using the GNU Compiler. One of the tools in MinGW is GCC, which is the compiler for C, C++, and several other computer languages. There are several different build versions of MinGW. The "MinGW-Builds" project was selected because it appears to be the most widely used and supported. The posix version was selected instead of the win32 version as the latter does not support C++11 threads.
Note: if using Windows 10 64-bit you will need to install the 64-bit version of MinGW.

7-Zip 

http://www.7-zip.org/

7-Zip is a file archiver.

Software Tools Installation

Install 7-Zip, MSYS, and MinGW64. Afgter installing the 7-Zip utility, Use it to expand msys+7za+wget+svn+git+mercurial+cvs-rev9.7z to c:\msys and x64-4.8.1-release-posix-seh-rev5.7z to c:\mingw64. Modify the Path system variable to include the c:\mingw64\bin directory. In Windows 10, right click on the start button, select Control Pannel, select System and Security, select System, select Advanced system settings, click on the Environment Variables button, click on the Path system variable and press the Edit button. Add "c:\mingw64\bin;" to the Path.

Build Copper Spice

After the software tools are installed and configured, run the msys.bat file located in the c:\msys directory by right clicking it with file explorer and selecting "Run as administrator". A MINGW Box will open. Pull down the source code and samples by using the git utility and use the GNU Autotools Build System to build from the source code. We have selected the $HOME directory for installation as convenience. This will take several hours to complete. 
In the MINGW Box issue the commands:

$ cd $HOME

$ mkdir git

$ cd git

$ git clone https://github.com/wpbest/copperspice.git

$ cd copperspice

$ ./autogen.sh

$ mkdir build

$ cd build

$ ../configure --prefix $HOME

$ make

$ make install

Build Copper Spice Samples

Build kitchensink

$ cd $HOME/git/copperspice/samples/kitchensink

$ ./autogen.sh

$ mkdir build

$ cd build

$ ../configure --prefix $HOME

$ make

$ make install

Build screenshot

$ cd $HOME/git/copperspice/samples/screenshot

$ ./autogen.sh

$ ./configure --prefix $HOME

$ make

$ make install

Build settingseditor

$ cd $HOME/git/copperspice/samples/settingseditor

$ ./autogen.sh --prefix $HOME

$ ./configure --prefix $HOME

$ make

$ make install

Build simple

$ cd $HOME/git/copperspice/samples/simple

$ ./autogen.sh

$ ./configure --prefix $HOME

$ make

$ make install

Build simpletextviewer

$ cd $HOME/git/copperspice/samples/simpletextviewer

$ ./autogen.sh

$ ./configure --prefix $HOME

$ make

$ make install

The sample application executables will be located in $HOME/exe or c:\msys\home\'username'\bin.
Note: The kitchensink executable is located in $HOME/git/copperspice/samples/kitchensink/exe or c:\msys\home\'username'\git\copperspice\samples\kitchensink\bin.
Note: 'username' is the user you are logged into the windows operating system. i.e. wpbest is my user name in my case.
