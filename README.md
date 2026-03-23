# Gameplan

- [x] Json structure holding data

Json holds:

- [x] Discord token
- [x] Alert channel ID
- [x] Post channel ID
- [x] offset
- [x] prio-queue (array, see queue holds for content)
- [x] queue (array, see queue holds for content)

queue holds:

- [x] pet-name
- [x] user-id (<@\{id}>)
- [x] photo-url (local)

### 
- [x] persistant program

Commands of persistant program:

- [x] add :: add cat to queue (requires picture file, user id and cat name)
- [x] addprio :: add cat to prio-queue (requires picture file, user id and cat name)
- [x] remove :: removes an entree from queue or prio-queue BY INDEX
- [x] list :: list all current queue and prio-queue entrees (name and user_ID only)
- [x] getpicture :: get picture from entree BY INDEX
- [x] help :: stupid

###
- [x] Get picture from command use (save to disc, wget Discord URL?)

# Folder structure

```
|
+-.vscode-+
|         +-launch.json
|         +-settings.json
|         +-tasks.json
|
+-build-+
|       +-update_app
|       +-manager_app
|       +-COTD_queue.json
|
+-lib-+
|     +-json.hpp
|
+-hdr-+
|     +-queue-manager-+
|     |               +-main.h
|     |
|     +-queue-update-+
|     |              +-main.h
|     |
|     +-json.h
|     +-common.h
|
+-src-+
|     +-queue-manager-+
|     |               +-main.cpp
|     |
|     +-queue-update-+
|     |              +-main.cpp
|     |
|     +-json.cpp
|
+-.gitattributes
+-.gitignore
+-README.md
```