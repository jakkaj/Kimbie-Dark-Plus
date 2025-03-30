
# Enhancing Kimbie Dark+ for Better C++ Syntax Support

## Goal of the Theme Modification  
The goal is to update the **Kimbie Dark+** Visual Studio Code theme so that it fully supports the richer C++ syntax scopes provided by the **Better C++ Syntax** extension. We want to **retain all of Kimbie Dark+’s existing color aesthetics** while introducing additional TextMate scope mappings for C++ code. This will ensure that new C++ language constructs (like function parameters, cast operators, etc.) are highlighted consistently with the theme’s style, rather than appearing in the default (uncolored or miscolored) style. In essence, we’ll be **overriding or adding token color rules** for the new C++ scopes – using the same palette Kimbie Dark+ already uses – to achieve more nuanced syntax coloring.

## Better C++ Syntax vs. Default C++ Grammar  
The **Better C++ Syntax** extension replaces VS Code’s default C++ TextMate grammar with an improved one. This enhanced grammar provides **finer-grained scopes** for C++ code elements that were previously indistinguishable. According to the extension’s documentation, it fixes numerous highlighting issues and *adds* support for things like: 

- **Function parameter highlighting** – parameters in function declarations/definitions get a distinct scope.  
- **Lambda function highlighting** – lambdas and their captures are identified with unique scopes.  
- **Function pointer syntax** – function pointer declarations are scoped separately.  
- **Casting operators** – `static_cast`, `dynamic_cast`, `reinterpret_cast`, `const_cast` are given specific operator scopes.  
- **User-defined literals and C++14 digit separators** – e.g. `100'000ms` are properly tokenized.  
- **Template syntax and parameters** – template parameters and template calls get more specific scopes.  
- **C++20 features** – new syntax (like `<=>`, concepts, module import) are accounted for (ongoing improvements).  
- **Various keywords and operators** – distinguishes keywords like `override`, `final`, `constexpr`, `mutable`, `nullptr`, etc., and fixes misclassification of `and`, `or`, etc..  

In contrast, the **default C++ grammar** lumped many of these into generic scopes. For example, without the extension, a function parameter name might just be scoped as a regular variable, and a cast like `static_cast` might not be distinctly scoped at all. Better C++ Syntax exposes these as separate scopes (e.g. `variable.parameter.cpp` for parameter names in C++ and `keyword.operator.cast.static_cast.cpp` for the cast) so that themes can colorize them differently ([Some keywords are not recognized · Issue #535 · jeff-hykin/better-cpp-syntax · GitHub](https://github.com/jeff-hykin/better-cpp-syntax/issues/535#:~:text=,keyword.other.using.directive.cpp)). 

**Why the theme needs updating:** Kimbie Dark+ (as originally designed) didn’t anticipate these new scopes, since the extension became the “official” grammar later. As a result, out-of-the-box Kimbie Dark+ may not apply any special color to, say, a parameter name or the `static_cast` keyword. By modifying the theme to include rules for the new scopes, we can ensure those tokens get colored in a way that matches Kimbie Dark+’s style (instead of appearing in the default color). Notably, the Noctis theme (which is recommended for use with Better C++ Syntax) already includes such mappings – we will use its approach as a guide.

## New C++ Scopes and Kimbie Dark+ Color Mapping  
Below is a list of **additional C++ token scopes** introduced by Better C++ Syntax, along with **descriptions** and the **suggested Kimbie Dark+ color** for each. The colors are chosen to match existing Kimbie Dark+ styling of similar constructs, ensuring the enhancements feel native to the theme. (We also note how the Noctis theme handles or groups these scopes, to validate our choices.)

- **Function Parameters** – *Parameter names in function declarations and definitions.*  
  **Scopes:** `variable.parameter.cpp` (and the older `variable.parameter.function` scope).  
  **Kimbie Dark+ Color:** **Beige** (`#d3af86`). Kimbie already uses a beige tone for function parameters ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), so we’ll apply the same to the new scope. In Kimbie Dark+, this beige is used for various “secondary” text like parameters and punctuation. Noctis likewise gives function parameters a distinct color, aligning with its semantic color design (we keep Kimbie’s established beige here).  

- **Cast Operators** – *C++ casting keywords.* This includes `static_cast`, `dynamic_cast`, `reinterpret_cast`, and `const_cast`.  
  **Scope pattern:** `keyword.operator.cast.*.cpp` (each cast has its own full scope).  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). In Kimbie, most C++ keywords are this brown/purple color ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). The theme already treats `new` and `delete` as keywords and colors them with the same tone ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). We will similarly color all cast keywords as regular keywords. This means they will stand out with the brownish hue (rather than the default plain color). *Noctis*, for example, also styles these cast operators in line with other keywords to ensure they don’t blend in.

- **`override`, `final` (and other function specifiers)** – *Contextual specifiers for class methods.* These appear after function parameter lists to modify virtual functions.  
  **Scopes:**  
  – `storage.modifier.specifier.functional.post-parameters.override.cpp` (for `override`)  
  – `storage.modifier.specifier.functional.post-parameters.final.cpp` (for `final`)  
  – (`override` and `final` might also be tagged more generally as `storage.modifier.specifier` in the scope chain).  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). These are essentially keywords indicating special behavior, so we map them to the same color as other “storage/specifier” keywords (which in Kimbie are colored as keywords). Kimbie’s existing rule for `storage` covers keywords like `static` and `const` in that brown/purple tone ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). We will extend that to `override`/`final` as well. (The Noctis theme similarly gives `override` and `final` a keyword-like color, ensuring they are not missed – in Noctis, all these specifiers get a consistent treatment to convey their semantic role.)

- **`constexpr`, `explicit`, `mutable`, `virtual` (function qualifiers)** – *Function declaration prefixes.* These keywords appear before function definitions (e.g. `constexpr int func(...);`).  
  **Scopes:** e.g. `storage.modifier.specifier.functional.pre-parameters.constexpr.cpp`, `...explicit.cpp`, `...mutable.cpp`, `...virtual.cpp`. (Also `inline` in function context as `storage.modifier.specifier.functional.pre-parameters.inline.cpp`.)  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). These are also “storage specifiers” in C++ and should use the same color as other storage-class keywords. By default Kimbie Dark+ colors all `storage` keywords (like `static`, `extern`, etc.) with that brown tone ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), so we will apply the same to these newer qualifiers. In effect, all these function specifiers will remain the normal keyword color – which is exactly how Noctis handles them as well (grouping them with other keywords for consistency).

- **`using` directive** – *Namespace import directive.* The Better C++ grammar uses a scope that might be slightly different from the default. For example, it may tag `using namespace std;` as `keyword.other.using.directive.cpp` (instead of the older `keyword.other.directive.using`).  
  **Scope:** `keyword.other.using.directive.cpp`  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). Kimbie already colors the `using` keyword as a keyword (brown) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), so we ensure the `.cpp` qualified scope also gets the same. (Noctis doesn’t change the fundamental treatment of `using` – it remains a keyword in appearance.)

- **`operator` keyword (in operator overloads)** – *The use of the `operator` keyword in defining or declaring overloaded operators.* e.g. `struct X { operator bool() const; };`.  
  **Scope:** `keyword.other.operator.overload.cpp`  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). This is a keyword usage, so we align it with Kimbie’s keyword color. (This scope wasn’t explicitly in the original theme; adding it ensures `operator` in this context isn’t left uncolored. In Noctis, this gets no special color beyond being a keyword, which is what we’re doing for Kimbie Dark+ as well.)

- **Memory allocation keywords** – *The `new` and `delete` operators.* (These were already present in older grammar, but worth noting.)  
  **Scopes:** `keyword.operator.new.cpp`, `keyword.operator.delete.cpp`  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`). Kimbie Dark+ already lists these under its “Keywords” rule to color them brown ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). (Noctis likewise doesn’t treat them as mere operators – they are colored as keywords, standing out from generic operators.) We don’t actually need to change anything here, since Kimbie already handles them; just ensure we keep these entries when modifying the theme.

- **Boolean and Null literals** – *The constants `true`, `false`, and `nullptr`.* (The extension likely tags `nullptr` as `constant.language.nullptr.cpp` and `true/false` as `constant.language.boolean`.)  
  **Scope:** `constant.language.bool` / `constant.language.nullptr` (with `.cpp` suffix potentially).  
  **Kimbie Dark+ Color:** **Orange** (`#f79a32`). In Kimbie, all constants (numbers, booleans, etc.) use an orange/yellow tone ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). The theme’s “Constants” rule already captures booleans and `nullptr` via the `constant` scope and assigns this color. We should verify that `nullptr` is indeed recognized as a constant; if not, add a rule for `constant.language.nullptr.cpp` mapping to the same orange. (Noctis gives literal constants a distinct color too; for Kimbie we stick to its established orange for numeric and boolean literals.)

- **The `this` pointer** – *The implicit object pointer in C++.* Better C++ Syntax tags `this` as a language variable: `variable.language.this.cpp`.  
  **Scope:** `variable.language.this.cpp`  
  **Kimbie Dark+ Color:** **Red** (`#dc3958`). Kimbie Dark+ normally colors variables and object names in a reddish tone ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). Since `this` is essentially a predefined variable, we keep it styled as such. The existing “variable” rule in Kimbie will already match and color `this` (because the scope starts with `variable`), but we can explicitly ensure it by adding a rule if needed. (Some themes choose to highlight `this` like a keyword, but to maintain Kimbie’s feel, treating it as a regular variable is appropriate. Noctis, for instance, doesn’t overly accentuate `this` beyond giving it the variable color in its palette.)

- **Function Pointers and Declarator Operators** – *Function pointer declarations and dereference (`*` in types), etc.* The extension may provide scopes like `storage.type.function.pointer.cpp` for the function pointer asterisk, or improve how `*` and `&` in declarations are scoped.  
  **Scopes:** (if present) `storage.type.function.pointer.cpp`, `storage.modifier.pointer.cpp`, etc.  
  **Kimbie Dark+ Color:** **Brownish purple** (`#98676a`) for `storage.type` (since types in C++ are often treated as storage keywords or class names), or use the appropriate existing color. For example, the `*` in a function pointer might just inherit the type or storage color. We’ll mirror Noctis’ approach: ensure these new scopes don’t stand out incorrectly. In practice, built-in types (`int`, `float`) are `storage.type` which Kimbie already colors as a keyword (brown), and user-defined types use the class color (orange) via `entity.name.type`. The extension doesn’t change those fundamentals, so no new rule needed unless a specific new scope appears. (It’s worth scanning with the scope inspector to see if any uncolored token appears for complex pointer syntax and then assign it accordingly.)

- **Scope Resolution Operator `::`** – *Used in qualifiers (e.g. `NS::Class::member`).* The improved grammar ensures proper tokenization of `::`. Typically, `::` is scoped as `punctuation.separator.namespace` or similar.  
  **Scope:** `punctuation.separator.scope-resolution.cpp` (or it might remain `keyword.operator.scope-resolution`).  
  **Kimbie Dark+ Color:** **Beige** (`#d3af86`). In Kimbie, general operators and punctuation (., ::, +, etc.) are colored beige ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). We will continue that: if `::` gains a more specific scope name, we map it to the same operator/punctuation color. (Noctis likely doesn’t deviate for `::` – it will be colored akin to other operators in that theme.)

*Table: New C++ Scopes and Suggested Colors (Kimbie Dark+)*

| **Scope (TextMate selector)**                                   | **Meaning**                           | **Kimbie Dark+ Color**        |
|---------------------------------------------------------------  |---------------------------------------|------------------------------|
| `variable.parameter.cpp` (also `variable.parameter.function`)   | Function parameter names              | Beige `#d3af86` (same as existing parameter color) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| <small>`meta.parameter.cpp` (if styling full param is desired)</small> | <small>Entire function parameter (type + name)</small> | <small>Beige `#d3af86` (optional, usually type is colored separately)</small> |
| `keyword.operator.cast.static_cast.cpp`<br>`keyword.operator.cast.dynamic_cast.cpp`<br>`keyword.operator.cast.reinterpret_cast.cpp`<br>`keyword.operator.cast.const_cast.cpp` | C++ cast operators (`static_cast`, etc.) | Brown/Purple `#98676a` (treat as keywords) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `keyword.operator.new.cpp`<br>`keyword.operator.delete.cpp`     | Memory allocation operators           | Brown/Purple `#98676a` (already in Kimbie keywords) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `keyword.other.using.directive.cpp`                             | `using` namespace directive           | Brown/Purple `#98676a` (keyword) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `keyword.other.operator.overload.cpp`                           | `operator` keyword in overloads       | Brown/Purple `#98676a` (keyword) |
| `keyword.other.default.constructor.cpp`                         | `= default` (defaulted function)      | Brown/Purple `#98676a` (keyword) |
| `storage.modifier.cpp` (general) <br>`storage.modifier.static.cpp`<br>`storage.modifier.inline.cpp` | Storage-class specifiers (static/inline at variable declarations) | Brown/Purple `#98676a` (storage keyword) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `storage.modifier.specifier.*.cpp` (general pattern)            | Function specifiers (`override`, `final`, `constexpr`, etc.) | Brown/Purple `#98676a` (storage/keyword) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `storage.modifier.lambda.mutable.cpp`                           | `mutable` on lambda capture           | Brown/Purple `#98676a` (keyword) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `variable.language.this.cpp`                                    | `this` pointer                        | Red `#dc3958` (same as variables) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| `constant.language.nullptr.cpp`<br>`constant.language.boolean.cpp` | `nullptr`, `true`, `false` constants | Orange `#f79a32` (same as other constants) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| *(`entity.name.scope-resolution.cpp`)*<br>*(`punctuation.separator.scope-resolution.cpp`)* | *Scope resolution `::` operator*      | Beige `#d3af86` (same as other operators) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) |
| *(`storage.type.function.pointer.cpp`)* (if present)            | *Function pointer declarator (asterisk as part of type)* | Brown/Purple `#98676a` (same as `storage`/type keywords) |

**Notes:**  
- *We use the same hex colors already defined in Kimbie Dark+.* For reference: **Keywords/Storage** are `#98676a`, **Operators/Punctuation/Parameters** are `#d3af86`, **Variables** are `#dc3958`, **Classes/Types** are orange `#f06431` (for user types) or the same brown for built-in types, **Strings** are olive `#889b4a`, **Constants/Literals** are bright orange `#f79a32`, etc ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)). We are not introducing new colors—only reusing these so that the new tokens “blend in” with Kimbie’s style.  

- *Why treat cast operators as keywords?* Kimbie Dark+ (like many themes) differentiates between **operators** (arithmetic, logical, etc.) and **keywords**. In the default theme, cast keywords were not highlighted distinctively. We choose to color `static_cast` & co. as keywords (brown/purple) rather than as generic operators (beige) because semantically they behave more like control keywords. (Kimbie already did this for `new`/`delete`, and Noctis does similarly.) This makes them more visible in code, which is likely desirable.

- The **storage specifiers** (`static`, `constexpr`, etc.) all remain the same color as other keywords in Kimbie. If one wanted to, one could differentiate them (for example, some themes give `const` a different color to highlight immutability). However, since the goal is **not** to alter the theme’s design, we’ll keep them uniform. (Kimbie didn’t originally separate these, and our spec won’t either. Noctis, for instance, keeps a consistent style but might use a distinct hue for some specifiers due to its “semantic meaning” approach – here we stick to Kimbie’s unified style).

- Scopes marked in *italics* in the table are optional or already covered by existing rules. For instance, `entity.name.scope-resolution` for `::` might already fall under a broader rule (or could be treated as punctuation). We include them for completeness; you can decide if an explicit rule is needed once you test.

## Implementation Steps (Modifying the Theme JSON)  
To integrate these changes, we will modify the **Kimbie Dark+ theme JSON** (usually named `kimbie-dark-color-theme.json` in the extension). We will add new entries under the `"tokenColors"` array for the above scopes. Here’s a step-by-step guide:

1. **Locate the Theme File:** If you installed Kimbie Dark+ from the marketplace, you can find the theme file in your VS Code extensions directory (e.g. on Windows: `%USERPROFILE%\.vscode\extensions\dnamsons.kimbie-dark-plus-<version>\themes\kimbie-dark-color-theme.json`). Open this JSON file in an editor. Alternatively, you can copy its content into a new custom theme JSON if you prefer not to edit the extension directly.

2. **Preserve Existing Rules:** Make sure all the original token color rules remain intact – we’ll only be **adding or overriding** a few specific ones. Kimbie Dark+ already has rules for things like `"keyword"`, `"variable"`, `"string"`, etc., which we won’t touch. 

3. **Add New TokenColor Entries:** Below the existing rules, insert new JSON objects for the Better C++ Syntax scopes. You can group some together if they share the same color. Use the **scope names** and **colors** from the table above. For example, to add cast operator coloring and parameter coloring, you might insert something like:

   ```js
   {
       "name": "C++ Cast Operators (static_cast, etc)",
       "scope": [
           "keyword.operator.cast.static_cast.cpp",
           "keyword.operator.cast.dynamic_cast.cpp",
           "keyword.operator.cast.reinterpret_cast.cpp",
           "keyword.operator.cast.const_cast.cpp"
       ],
       "settings": {
           "foreground": "#98676a"
       }
   },
   {
       "name": "C++ Function Parameters",
       "scope": [
           "variable.parameter.cpp"
       ],
       "settings": {
           "foreground": "#d3af86"
       }
   },
   {
       "name": "C++ Override/Final Specifiers",
       "scope": [
           "storage.modifier.specifier.functional.post-parameters.override.cpp",
           "storage.modifier.specifier.functional.post-parameters.final.cpp",
           "storage.modifier.specifier.functional.pre-parameters.constexpr.cpp",
           "storage.modifier.specifier.functional.pre-parameters.inline.cpp",
           // ... (you can list all specifier scopes, or use a broader selector as below)
       ],
       "settings": {
           "foreground": "#98676a"
       }
   },
   {
       "name": "C++ Misc Keywords",
       "scope": [
           "keyword.other.using.directive.cpp",
           "keyword.other.operator.overload.cpp",
           "keyword.other.default.constructor.cpp"
       ],
       "settings": {
           "foreground": "#98676a"
       }
   },
   {
       "name": "C++ 'this' pointer",
       "scope": "variable.language.this.cpp",
       "settings": {
           "foreground": "#dc3958"
       }
   }
   ```

   A few notes on the above example:
   - We listed each cast operator explicitly under the same rule (since wildcards aren’t directly supported in the JSON). This ensures all four cast keywords get colored.  
   - For the specifiers like override/final/constexpr, you have two options:
     - **Explicit listing:** List every new scope (as shown, though abbreviated). This is straightforward but long.  
     - **Scope prefix selector:** VS Code theme files do allow space-separated scope selectors to target patterns. For instance, you could use `"scope": "source.cpp storage.modifier.specifier"` to target any specifier in C++. Or even just `"storage.modifier.specifier"` might match all of those (since Kimbie doesn’t otherwise use that combo). Using a broader selector means if Better C++ Syntax adds another specifier scope in the future, it will still match. The Noctis theme likely uses such patterns to avoid listing dozens of scopes. For clarity, you might stick with the explicit list now, but know that `"storage.modifier.specifier"` (with the `.cpp` omitted) could catch all of them as well.  
   - The `"name"` field is optional (it’s just a label for your reference). What matters is the `"scope"` and `"settings"`. Ensure the color hex codes match the ones from Kimbie’s palette.
   - If a scope was already present in Kimbie’s JSON, adding a new rule for it will override the previous one if placed later in the file. For example, Kimbie already has a rule for `"variable"` that makes variables red; by adding a more specific `"variable.language.this.cpp"` rule after it, the `this` pointer will follow our new rule without affecting other variables.

4. **Save and Reload:** Save the modified theme file. If you edited the extension’s file, you may need to reload/restart VS Code for it to pick up changes. If you created a custom theme JSON (in your `settings.json` via `workbench.colorCustomizations` or as a new theme extension), select that theme in VS Code.

5. **Verify the Highlighting:** Open a C++ file (with Better C++ Syntax extension enabled) and test various code constructs:
   - Write a function with parameters and see that parameter names are beige, distinct from other variables. For example:  
     ```cpp
     void example(int count, const std::string& name) { 
         // inside function, use 'count' and 'name' 
     }
     ```  
     In the signature, `count` and `name` should appear in the beige parameter color. (Inside the function body, they may appear red like normal variables, which is expected since the TextMate grammar doesn’t differentiate them there without semantic highlighting.)  
   - Use each of the cast operators (`static_cast`, etc.) in code and ensure they are colored purple/brown, similar to a keyword like `if` or `while` (and not plain white). For example: `float f = static_cast<float>(myInt);` – `static_cast` should be brownish-purple.  
   - Mark a method as override/final and see that the `override`/`final` keyword is colored. e.g.  
     ```cpp
     struct Base { virtual void foo(); };
     struct Derived : Base { void foo() override; };
     ```  
     The `override` should now be purple instead of the default text color.  
   - Try `constexpr`, `mutable` (in a lambda or as a member), `explicit` on a constructor, etc., to confirm they all show as keywords. For instance: `constexpr int x = 5;` – `constexpr` should be purple. Or a lambda: `[x]() mutable { return x++; };` – `mutable` should be purple.  
   - Check that `this` is red like other variables, and `nullptr` is orange like a constant. e.g. `if (this != nullptr) { … }`.  
   - Ensure normal operators (`+`, `-`, `<<`, etc.) remain beige, and strings, comments, numbers etc. are unaffected (they should remain their usual Kimbie colors).

6. **Use the Scope Inspector:** VS Code has a very useful tool to confirm scope mappings. Open the Command Palette and run **“Developer: Inspect Editor Tokens and Scopes”**. Click on any token in your C++ code and you’ll see a popup listing the TextMate scopes applied to that token, and which theme rule is coloring it. Use this to verify that:  
   - The scopes from Better C++ Syntax are indeed present for the token (e.g. clicking on a parameter name shows `variable.parameter.cpp` in the scope chain, clicking on `static_cast` shows `keyword.operator.cast.static_cast.cpp`, etc.).  
   - The expected color rule from your theme is being applied. The inspector will show which theme rule matched. If it’s not the one you added, you may need to adjust the scope selector or ensure your rule is last. For example, if `override` still isn’t colored, the inspector might reveal it has scope `storage.modifier.specifier.override.cpp` but perhaps our rule had a slightly different selector. You can then tweak the rule (maybe remove the `.functional.post-parameters` part and just target `specifier.override`). The inspector helps pinpoint such mismatches.

## Tips and Caveats  
- **Avoid Duplicate Definitions:** Be careful not to duplicate an existing scope in two rules with different colors. The later rule in the file will win, which is what we rely on for overrides. For instance, Kimbie’s original had a generic `"keyword"` rule ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)); by adding a specific `"keyword.operator.cast"` rule later, you override the generic for those cast keywords. This is fine. Just ensure consistency – e.g., don’t accidentally give `const_cast` a different color in another rule. Keep all our new scopes aligned with the intended colors.

- **Semantic Highlighting Interaction:** C++ can also be colored via VS Code’s **Semantic Token** system (provided by the C++ IntelliSense extension). Kimbie Dark+ notes “support for semantic syntax highlighting”. If semantic highlighting is enabled, it might color certain tokens (like class names, parameters, etc.) according to semantic rules, potentially overriding TextMate theme colors. You have a few options:
  - Since our focus is on the TextMate grammar provided by Better C++ Syntax, you might want to **disable semantic highlighting** for C++ to clearly see your theme changes. This can be done by setting `"C_Cpp.enhancedColorization": "Disabled"` in your VS Code settings (for the MS C++ extension), or turning off semantic token colorization in the theme (some themes have a `"semanticHighlighting": false` setting).  
  - Alternatively, you can **extend the theme’s semantic token rules** to match these colors. However, that’s a separate layer. (For example, semantic tokens have a token type for `static` or for parameters – you could map those to the same hex colors via the theme’s `semanticTokenColors` or `editor.semanticTokenColorCustomizations` in settings.) Noctis likely covers both layers. If you notice your changes aren’t showing, semantic might be taking precedence, so check that. For the scope of this task, focusing on TextMate scopes is sufficient, assuming Better C++ Syntax is active and semantic highlighting is either off or not conflicting.

- **Testing with Noctis as Reference:** If you want to see a working example in action, try installing the Noctis theme alongside Better C++ Syntax and open the same C++ code. Observe how it colors `override`, parameters, etc. – you should see that everything is colored in a deliberate way (no plain white identifiers where they shouldn’t be). Our aim is to achieve the same coverage, but with Kimbie’s colors. If something in your theme still looks plain or out-of-place, double-check if there’s an unhandled scope. The extension’s documentation or the scope inspector can help identify what scope to target. 

- **Maintainability:** If the Better C++ Syntax extension updates with new scopes (for future C++ features), you’d need to update the theme accordingly. To minimize maintenance, you can use **broader scope selectors** as mentioned. For instance, using `"storage.modifier.specifier"` without the language suffix will catch any specifier in any language (which in practice mostly affects C++ here). Since Kimbie’s palette is language-agnostic for such categories, that’s usually fine. Jeff Hykin (the extension author) suggests using broader selectors without “.cpp” when possible, for future compatibility. Just be cautious that it doesn’t unintentionally affect other languages. In our case, scoping to `source.cpp` in the selector (e.g. `"source.cpp storage.modifier.specifier"`) can ensure it only targets C++ code.

- **Backup the Original Theme:** If you are directly editing the theme file provided by the extension, note that updating the extension in the future could overwrite your changes. You may want to keep a backup of the modified JSON or increment the version (if you’ve copied it to a custom theme). Another approach is to use the **`"editor.tokenColorCustomizations"`** in your user settings to override theme colors without editing the file. You could place the same rules under your settings, targeting the Kimbie theme by name. For example:  
  ```json
  "editor.tokenColorCustomizations": {
    "[Kimbie Dark+]": {
      "textMateRules": [
        {
          "scope": "variable.parameter.cpp",
          "settings": { "foreground": "#d3af86" }
        },
        // ... other rules
      ]
    }
  }
  ```  
  This way, your customizations live in your settings and will apply on top of the installed theme. It’s up to you – since the task is about a “specification”, you might be implementing it in the theme source itself.

By following the above steps, you will integrate Better C++ Syntax’s enhancements into Kimbie Dark+ smoothly. The end result will be C++ code that is colored richly: function parameters will be distinguishable, modern C++ keywords won’t be missed, and overall the theme will feel “C++20-aware” without losing the classic Kimbie Dark+ vibe. Enjoy your improved syntax highlighting! 

**Sources:**

- Better C++ Syntax extension README – highlighted the additional C++ grammar features (parameters, lambdas, etc.).  
- Kimbie Dark+ theme JSON – used to confirm the original color mappings for keywords, variables, etc., ensuring our additions use the same palette (e.g. parameters in Kimbie are beige ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), keywords like `if` or `new` are purple/brown ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)) ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), punctuation/operators are beige ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), constants are orange ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), variables are red ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=)), and storage keywords are purple/brown ([vscode/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json at main · microsoft/vscode · GitHub](https://github.com/Microsoft/vscode/blob/master/extensions/theme-kimbie-dark/themes/kimbie-dark-color-theme.json#:~:text=))).  
- Jeff Hykin’s comments on theme overrides – guidance on using broad scope selectors for maintainability.  
- Visual Studio Marketplace recommendation – Noctis theme is recommended to benefit from Better C++ Syntax’s improved scopes, indicating it includes mappings for these new tokens (which we emulate in Kimbie’s style).