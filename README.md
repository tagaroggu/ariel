# Ariel
Ariel: **A**nother **R**eact-**I**nspired/-**E**sque **L**ibrary. Alternatively, Ariel: HTML in the C.

<!--View examples in the examples folder, or such documentation below.-->

## Basic usage (with macros)
Ariel defines 3 main macros that expose the base functionality of the library: e, p, and t, which stand for **e**lement, **p**roperty, and **t**ext respectively. The e macro lets users pass in blocks of code that can mix both C and Ariel macros to build HTML.

```c
#include <stdio.h>

#define ARIEL_IMPLEMENTATION // Ariel is a header-only library, define this only once in your code!
#include "ariel.h"

int main() {

    char *myElement = e("myElement", { p("lang", "en-US") }, {
        e("childElement", {}, {
            t("This is my text.")
        })
    });

    printf("%s", myElement); // <myElement lang="en-US"><childElement>This is my text.</childElement></myElement>
    return 0;
}
```

As well, since the two blocks of the e macro are just executed one after another and are otherwise identical, you can define ARIEL_JOIN_BLOCKS to have just one block instead. You could even mix and match the e, p, and t macros however you want.

```c
#include <stdio.h>

#define ARIEL_IMPLEMENTATION
#define ARIEL_JOIN_BLOCKS // This just has to go before the include, otherwise order doesn't matter
#include "ariel.h"

int main() {

    char *myElement = e("myElement", {
        e("childElement", {
            t("This is my text.")
        })
        p("lang", "en-US")
    });

    printf("%s", myElement); // <myElement lang="en-US"><childElement>This is my text.</childElement></myElement>
    return 0;
}
```
## Basic usage (without macros)
Ariel defines a number of functions under the hood:
```c
void __ArielPushStack(struct Element *element);
struct Element *__ArielPopStack();
void __ArielOpenBlock(struct Element *element);
char *__ArielCloseBlock();

struct Element *createElement(char *name);
void appendChild(struct Element *parent, char *child);
void appendProperty(struct Element *element, char *property);
char *ElementToString(struct Element *element);
void freeElement(struct Element *element);

char *createProperty(char *name, char *value);

char *createText(char *text);
```

As well, Ariel has the Element struct:
```c
struct Element {
  char *name;
  char *properties;
  char *children;
};
```

And Ariel's macros are define like so:
```c
#ifndef ARIEL_JOIN_BLOCKS
#define e(name, propsBlock, childrenBlock) ({ __ArielOpenBlock(createElement(name)); propsBlock; childrenBlock; __ArielCloseBlock(); });
#else
#define e(name, block) ({ __ArielOpenBlock(createElement(name)); block; __ArielCloseBlock(); });
#endif

#define p(name, value) ({ char *p = createProperty(name, value); appendProperty(__ARIEL_STACK->element, p); free(p); });
#define t(text) ({ char *t = createText(text); appendChild(__ARIEL_STACK->element, t); free(t); });
```

An example of using Ariel without the macros may look something like this:
```c
#include <stdio.h>

#define ARIEL_IMPLEMENTATION
#include "ariel.h"

int main() {
    struct Element *myElement = createElement("myElement");

    char *langProp = createProperty("lang", "en-US");
    appendProperty(myElement, langProp);
    
    struct Element *childElement = createElement("childElement");

    char *text = createText("This is my text.");
    appendChild(childElement, text);

    char *childElementString = ElementToString(childElement);

    appendChild(myElement, childElementString);

    char *htmlString = ElementToString(myElement);

    printf("%s\n", htmlString); // <myElement lang="en-US"><childElement>This is my text.</childElement></myElement>

    // Don't forget to free at some point!
    free(text);
    freeElement(childElement);
    free(childElementString);
    freeElement(myElement);

    return 0;
}
```

## To Dos
- [ ] Implement tests
- [ ] Implement examples
- [ ] Documentation

## Inspiration
I got the idea to do this from a few different things coming together.

First and foremost, I am working on rebuilding my knowledge of programming from the ground up with C. JavaScript has been my go-to for a long time, and it is a fairly comfortable language to me, but at the same time I get stuck in it sometimes. TypeScript does help smooth things over but even then sometimes I feel like I'm just fighting the compiler. Rebuilding my knowledge with C means I am getting types first and absolutely required, I am not beholden to browsers, and moreover I have been wanting to write in something more close to the bare metal, and C does offer that. In time, I will also touch on Rust again, but with good fundamentals from C that should hopefully make it easier for me than before.

Second, I am inspired by React, as the name of this project suggests. I myself am more of a Vue user, and Svelte would also still rank higher to me than React, but the simple, basic shape of react functional components (without hooks) is what inspired me to make this project. React isn't the only library to do this type of thing, even Vue does it, as well as Solid and Preact, but React fit the name that popped into my head better than the others; it's more provocative.

Third, tying back to my knowledge rebuilding, I am inspired by both Nic Barker's Clay project and Ramon Santamaria's/raysan5's Raylib project and associated projects. I can't quite explain why but they've been influential in helping me keep my head up in doing this, at least in part due to them being written in C as well. Clay's ClAY macro is an inspiration on the functionality of this library itself, though handled a bit differently. 

I think my fourth inspiration in this is back to webdev. Writing HTML and JavaScript was my first way into programming. I'll still write HTML, CSS, and JavaScript, and this project is very much an example of doing so. I just think I'll come out a better programmer by doing this as well.