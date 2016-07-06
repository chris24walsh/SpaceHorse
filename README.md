##**RELEASE VERSION NOW AVAILABLE**

https://www.dropbox.com/s/w4r70j10c8w71sx/SpaceHorse.zip?dl=0

Download and unzip the project to a suitable location. Navigate into 'SpaceHorse/game/Release/' and run SpaceHorse.exe.

###**Introduction**

This project was started with the goal of creating an open map, multiplayer, 2D space explorer. The first release candidate will aim to allow exploration of a universe, which is importable/configurable from a text file. Each planet/space station etc should be dockable with resources to mine/buy in order to level up the ship. This project is currently using C++ and the Allegro games library.


###**Setup**

If you want to develop for this project, you will need to install the allegro library - See instructions below. 

###**NEW INSTRUCTIONS FOR BUILDING IN VS COMMUNITY 2015**

##### 1. Download allegro

You can download Allegro 5.1 from the [Gna!] (http://download.gna.org/allegro/allegro-unstable-bin/5.1.12/) repository. Recommend that you download the 32 bit version - allegro-msvc2015-x86-5.1.12.zip. Select the x64 version only if you know what you are doing.

##### 2. Extract to disk

When it finishes downloading, extract the contents somewhere, e.g. C:\allegro5.1.

##### 3. Ensure solution properties are configured:

Open the Visual Studio solution file, right click on the project in Solution explorer, then click Properties:

At the top, where it says something like Configuration: Active(Debug), select All Configurations.

On the left menu select C/C++ -> General. On this configuration page, select Additional Include Directories and edit it to contain C:\allegro5.1\include\ in its text field.

On the left menu select Linker -> General. On this configuration page, select Additional Library Directories and edit it to contain C:\allegro5.1\lib\ in its text field.

Click on Apply at the bottom, but don´t close the window yet.
Back at the top, select the Debug configuration.

On the left menu select Linker -> Input. On this configuration page, select Additional Dependencies and edit it to contain allegro_monolith-debug.lib in its text field.

(Optional)On the left menu select Linker -> System. On this configuration page, select SubSystem and change it to Console from the drop-down box. This will give you a working terminal window in Debug mode to which you can write ...err... debug stuff ;)

Click on Apply at the bottom, but don´t close the window yet.

Back at the top, select the Release configuration.

On the left menu select Linker -> Input. On this configuration page, select Additional Dependencies and edit it to contain allegro_monolith.lib in its text field.

(Optional)On the left menu select Linker -> System. On this configuration page, select SubSystem and change it to Windows from the drop-down box. This will prevent the Terminal window to appear on Release versions of your program.

Click on Apply then OK, this will close the Property Pages of the project and should prepare it for Allegro.

##### 4. Any errors?

If you are getting errors when you rebuild the project, you may need to delete the allegroTest.sdf file located in ./allegroTest/ and reload the project (right click on the project in Solution Explorer). Contact me if you have trouble setting this up.


### Other resources

[Here] (https://wiki.allegro.cc/index.php?title=Windows,_Visual_Studio_2010_and_Allegro_5) is a link to a tutorial to set up VS with the [Allegro 5 windows binary] (https://www.allegro.cc/files/).
