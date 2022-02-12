# Assignment 

Please refer to Blackboard for full details on the assignment and its 
deliverables.

Project Layout
------
* app
  * ASGEGame: the game client and OpenGL Window
    * data: used for assets and game deployment
    * resources: used for icons 
  * Server: the dedicated server i.e. no window
    * data: used for server assets
    
* extern
  * important build scripts and other submodules
  * to manually update the submodules
    * `git pull --recurse-submodules`
    * `git submodule update --init --remote --recursive`
  
* include:
  * ASGEGameLib: public header files belonging to your game library and needed by your apps

* src 
  * ASGEGameLib/
  * **reusable game components go here i.e. dice, animated sprite, game panel, game state  etc** 
  * Private hpp files go here also i.e. not used by your apps
   
   
Static Analysis
------
If you want to resolve issues that GitHub builds have flagged, you can turn
on the live analysis. This will scan each file as it compiles it. Just don't
forget to turn it off when you're finished.

GameData
------
In order to access your game data from the game, you need to ensure that the +GD 
target has been run. You only need to run this when you've modified the contents
of your Data folder.
