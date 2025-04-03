# WmDP - Markdown Processor

Unconventional but highly performant markdown processor, WIP (Work in progress).

Look in the example directory to see a basic C usage example. To give a more basic understanding of how easy it is to use the library see the below code example.

```c
// Parse Markdown File (Output Ptr, Output Size, Input Ptr, Input Size);
psr(&r,&u,b,s);
```

## WebAssembly Usage
There is an embedded usage example in the wasm directory, you can compile the WebAssembly code with the following command.

```sh
// Code Here.
```

Afterwards launch a HTTP server in the corresponding directory and visit it on the localhost.

```sh
python3 -m http.server
```

Reference: [https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/C_to_Wasm]

## License
This project is licensed under the permissive MIT license. Please consider starring the project if you like it.
