This Unreal Engine project is the creation of a plugin that enables multiplayer functionality for any Unreal Engine game. The plugin establishes a server connection between players on a map of your choosing.
Simply click Host and you can Host a level of your choosing on a listen server.

**Check out the Lobby-UI branch for the latest updates on this project where the UI is getting revamped to show a list of lobbies and options for hosting. This new UI will be much more scalable for any project.**

If you'd like to utilize this plugin, simply download this repo as a zip. Then all you need to do is copy the Plugins folder over to another Unreal Engine project. Compatible with UE 5.0+ but only specifically tested on UE 5.3 and 5.4.
<---------------------------------------------------------------------------------------------------------------------------------->

Blank Project Tutorial
1)  Create new Unreal Engine 5 project.
2)  Create a map for the "Main Menu."
3)  Create a map for the "Lobby" where you wish to host the server. This will be the map all players travel to when the server is hosted.
4)  On your Main Menu level, open the Level Blueprint
5)  Within the level blueprint, add a Create Widget node and select the WBP_Menu widget from the plugin. Attach this to the Begin Play event.
-   Within the Menu Setup function, you can set a few parameters.
-   Number of Public Connections: Pretty self explanatory. Limits the limit of users in your lobby.
-   Type Of Match: Please keep this value to "FreeForAll" as it is hard-coded in the "Join" logic to only join a lobby with the Type Of Match "FreeForAll".
-   Lobby Path: Input the path to the level you wish to host the server on. The '/Game' directory is your Content folder. Just lead with '/Game' and replace the rest of the path with the corresponding path beginning     
        with, but not including, your Content folder. For instance, if you map is in Content/Maps/MyLobbyMap.umap, your value should be '/Game/Maps/MyLobbyMap'.
6)   Drag the output pin from the Create Widget node and call the "Menu Setup" function on the widget.
7)   Drag the output pin from the Create Widget node and create a "Add to Viewport" node. Make sure the widget is the target.
-   For the "Add to Viewport" node, you must get the Local Player Controller and plug it in to the player controller input pin.
8)    After adding the widget to viewport, you need to change the player's gamemode to UI Only utilizing the player controller from the previous step.
9)    Lastly, use the Set Show Mouse Cursor node to show the mouse cursor on the screen so the user can click the Host/Join buttons.


Picture of blueprint setup for reference:
![image](https://github.com/user-attachments/assets/c08e13c1-5b42-40d4-9786-02944b242cfd)


