# WmDP - Tiny & Fast Markdown Parser

Hello \`Hey` and `Hello`

\*Hey* `h` damn

Wew 
 * ewr
was

Testing...
  * Test
* Fresh Tomatoes
* Avocados
* Corn
  * Organic, healty and fairly nutritious
still it is tasty.
  * Locally sourced
    1. The *small* province
* Cabbage
* Wraps
  * Wheat

1. Revamp the codebase
2. Push changes to production
3. Clean the house
   1) Vacuum the carpets
      1. Basement and attic
   2) Wash the windows on the second floor
  
| Name          | Birthday     | Profession    |
| ------------- | :----------: | ------------: |
| Wolf Schulz   | 2005/07/09   | Programmer    |
| Ivy Schulz    | 2000/09/13   | Sales Agent   |
| Coen Schulz   | 1998/10/05   | Analyzer      |
| Anouk Dekker  | 1970/03/10   | Shaman        |
| Carl Schulz   | 1968/05/12   | IT Manager    |
| Name          | Birthday     | Profession    |
| ------------- | :----------: | ------------: |
| Wolf Schulz   | 2005/07/09   | Programmer    |
| Ivy Schulz    | 2000/09/13   | Sales Agent   |
| Coen Schulz   | 1998/10/05   | Analyzer      |
| Anouk Dekker  | 1970/03/10   | Shaman        |
| Carl Schulz   | 1968/05/12   | IT Manager    |

WIP (Work in progress).

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
