# :waffle: waffle

Just a simple custom X11 window Manager in C made by two bored friend to have 
some fun.

> **Note**
> If you want to contribute to the project, you're welcome! Just fork it, build
> your feature and make a pull request if you want to share it with everyone. 
 
The aim of the project is to learn C and X11, and have fun, and maybe one day 
making a viable product (*maybe*).

> **Warning**
> This is not at all a viable project yet. If you have planned to use it as your
> main WM, you can, but remember that the project is new, and we still have a 
> ton of things to do, and bugs to resolve.

## How to use

You can compile the `waffle` binary using the Makefile:
```shell
make
```

## Controls

Press <kbd>META</kbd> + <kbd>Enter</kbd> to open kitty (the default terminal) on
top of the other apps.

- Move windows around using <kbd>META</kbd> + <kbd>Left click</kbd> when 
  hovering a window. The window will go on top of the others.
- Resize windows using <kbd>META</kbd> + <kbd>Right click</kbd>. The window will 
  resize based on the corner you clicked.

## Development

> **Note**
> If you are using Nix flakes, you can use the flake.nix file.

There is 3 possible binaries that can be compiled with the Makefile:
- **waffle:** The normal one. You can use it in prod.
- **debug:** Like waffle, but with a lot more of logs in stdout.
- **angry:** Like debug, but with Libasan

```shell
make         # ./waffle
make debug   # ./debug
make angry   # ./angry
```
