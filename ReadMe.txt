Database of custom actors for Ocarina of Time. SharpOcarina 1.53 and above can connect to this database and install actors automatically in z64rom.

Guidelines if you want your custom actor to appear in this database:

- The actor and object IDs needs to be defined in the constants ACT_ID and OBJ_ID, and the included object in OBJ_H (all these defines need to be in the first 100 lines of a file, and can be either a .c or a .h file)

- Must have an actor.toml with its render and adjustable params

(check SwitchPillar if you need an example on these 2)

The following will be gone in a future update but for now they're needed:

- The actor cant use more than one custom object. It can use custom object + vanilla object however.

- The actor cant depend on other custom actors.

- 2 custom actors cant share the same custom object.


Join HylianModding and DM me if you