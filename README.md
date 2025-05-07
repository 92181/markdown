# WmDP - Markdown Processor

Highly performant markdown processor, WmDP is fairly conventional, however in edge cases results may differ from other parsers.

What WmDP can do...
* Tables, code blocks, checkboxes, subscript & superscript.
* Unordered, ordered lists & blockquotes.
* Italic, cursive, strike-through text.
* Images, links, titles, etc.

Look in the example directory to see a basic C usage example. To give a more basic understanding of how easy it is to use the library see the below code example.

```c
// Parse Markdown File (Output Size, Input Ptr, Input Size);
char *r=psr(&u,b,s);
```

## WebAssembly Usage
There is an embedded usage example in the wasm directory, you can compile the WebAssembly code with the following command.

```sh
emcc -O3 ./../md.c -o md.js -s EXPORTED_FUNCTIONS='["_psr","_malloc","_free","stringToUTF8"]'
```

Afterwards launch a HTTP server in the corresponding directory and visit it on the localhost.

```bash
python3 -m http.server
```

Reference: [https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/C_to_Wasm]

## License
This project is licensed under the permissive MIT license. Please consider starring the project if you like it.

Made by Wolf Pieter Schulz.
