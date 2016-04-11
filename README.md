###**Introduction**

This project was started with the goal of creating an open map, multiplayer, 2D space explorer. The first release candidate will aim to ~~have at least a local server possible, which will allow dynamic multiplayer play. Eventually, it would be great to have an online server to facilitate this~~ allow exploration of a universe, which is importable/configurable from a text file. Each planet/space station etc should be dockable with resources to mine/buy in order to level up the ship. This project is currently using C++ and two C libraries, the Allegro games library ~~and the Enet UDP networking library~~.


###**Setup**

If you want to develop for this project, you will need copies of the above libraries installed and will need to configure your project to link to these libraries. If you are using Visual Studio, you can simply put the libraries in "C:/allegro" and "C:/enet" respectively and a clone of this repo should already be linked correctly (I hope!). [Here] (https://wiki.allegro.cc/index.php?title=Windows,_Visual_Studio_2010_and_Allegro_5) is a link to a tutorial to set up VS with the [Allegro 5 windows binary] (https://www.allegro.cc/files/), and [Enet] (http://enet.bespin.org/Downloads.html) can linked in the same way.

###**NEW INSTRUCTIONS FOR BUILDING IN VS COMMUNITY 2015**

Step 1
We will download Allegro 5.1 from the [Gna!] (http://download.gna.org/allegro/allegro-unstable-bin/5.1.12/) repository. Download allegro-msvc2015-x86-5.1.12.zip. Select the x64 version only if you know what you are doing.

Step 2
When it finishes downloading, extract the contents somewhere, recommend C:\allegro5.1, or something.

Step 3
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

Step 4
If you are getting errors when you rebuild the project, you may need to delete the allegroTest.sdf file located in ./allegroTest/ and reload the project (right click on the project in Solution Explorer). Contact me if you have trouble setting this up.
