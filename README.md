# ush

           dMMmy+.                -ohmMMo                                   
          :MMMMMMMdoosyyhhhhyysosmMMMMMMN                                   
          /MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM`                                  
          .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMd                                   
         `yMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNo                                  
        `dMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMo                                 
        +MMMMMMNmdddmmNNNNNNNmmmddmNMMMMMMM.                                
        yMMMMMy-``--```......``.:.`./dMMMMM/                                
        yMMMMo  .dMMm:        oNMNs  `dMMMM/                                
        +MMMM`  sMMMMh       `MMMMM-  +MMMM.                                
        `mMMM-  :NMMMo        dMMMm`  sMMMh                                 
         -mMMd-  :sy/         `oyo. `/NMMh`                                 
          .odNNy/-.`           ``.:+dMNh/`                                  
            `-/shdddhhhhhhhhhhhddhdhs/.                                     
                 ``..-okulinich-..``


This is simple command line interpreter for Unix.
       contributors: @okulinich, @ailchuk

   **ush**:
* u - ucode
* sh - shell

You need clang compiler to be installed to compile this 
project, or you can change default compiler in Makefile.

Use `MAKE` to install and `./ush` to start shell.
`make uninstall` - uninstall shell
`make reistall` - uninstall & install shell

**ush** implements builtin commands:
- `export`
- `unset`
- `exit`
- `env` with `-i`, `-P`, `-u`
- `cd`  with `-s`, `-P`  and `-` argumen
- `pwd` with `-L`, `-P`
- `which` with `-a`, `-s`
- `echo` with `-n`, `-e`, `-E`

If command is not found among builtin ones, ush searches for
needed binary file in folders, specified by `$PATH` variable.

Shell manages signals `CTRL+D`, `CTRL+C`, `CTRL+Z`
and command separator `;`

These characters are escaped to be used literally:
`space`, `'`, `"`, `$`, `(`, `)`, `\\`, `{`, `}`.

Ush manages tilde expansion `~`, the basic form of parameter 
expansion `${parameter}`, command substitution ``command`` and `$(command)`.

Command editing is allowed with Arrow keys.

**Enjoy!**
