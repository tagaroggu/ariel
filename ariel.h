#ifndef ARIEL_H
#define ARIEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Element {
  char *name;
  char *properties;
  char *children;
};

struct __ArielStack {
  struct Element *element;
  struct __ArielStack *previous;
};

// TODO make sure this/these macros are correct
// #define e(name, propsBlock, childrenBlock) ({ createElement(name); __ArielOpenBlock(); propsBlock; childrenBlock; __ArielCloseBlock(); })

#ifndef ARIEL_JOIN_BLOCKS
#define e(name, propsBlock, childrenBlock) ({ __ArielOpenBlock(createElement(name)); propsBlock; childrenBlock; __ArielCloseBlock(); });
#else
#define e(name, block) ({ __ArielOpenBlock(createElement(name)); block; __ArielCloseBlock(); });
#endif

#define p(name, value) ({ char *p = createProperty(name, value); appendProperty(__ARIEL_STACK->element, p); free(p); });
#define t(text) ({ char *t = createText(text); appendChild(__ARIEL_STACK->element, t); free(t); });

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


#endif
#ifdef ARIEL_IMPLEMENTATION

//struct Element *__ARIEL_LAST_ELEMENT = NULL;
struct __ArielStack *__ARIEL_STACK = NULL;

void __ArielPushStack(struct Element *element) {
  struct __ArielStack *newStack = (struct __ArielStack *)malloc(sizeof(struct __ArielStack));
  newStack->element = element;
  newStack->previous = __ARIEL_STACK;

  __ARIEL_STACK = newStack;
}

struct Element *__ArielPopStack() {
  struct Element *element = __ARIEL_STACK->element;
  __ARIEL_STACK = __ARIEL_STACK->previous;

  return element;
}

/*void __ArielOpenBlock() {
  __ArielPushStack(__ARIEL_LAST_ELEMENT);
}*/

void __ArielOpenBlock(struct Element *element) {
  __ArielPushStack(element);
}

char *__ArielCloseBlock() {
  struct Element *element = __ArielPopStack();

  char *string = ElementToString(element);
  freeElement(element);

  if (__ARIEL_STACK == NULL) {
    return string;
  } else {
    appendChild(__ARIEL_STACK->element, string);
    free(string);

    return NULL;
  }
}

struct Element *createElement(char *name) {
  struct Element *output = (struct Element*)malloc(sizeof(struct Element));
  size_t nameLen = strlen(name) + 1;
  output->name = (char*)malloc(sizeof(char) * nameLen);
  strncpy(output->name, name, nameLen);

  output->properties = NULL;
  output->children = NULL;

  //__ARIEL_LAST_ELEMENT = output;

  return output;
}

void appendChild(struct Element *parent, char *child) {
  size_t childLen = strlen(child) + 1;
  if (parent->children == NULL) {
    parent->children = (char*)malloc(sizeof(char) * childLen);

    strncpy(parent->children, child, childLen);
  } else {
    size_t newChildrenLen = childLen + strlen(parent->children);
    char *temp = (char*)malloc(sizeof(char) * newChildrenLen);

    snprintf(temp, newChildrenLen, "%s%s", parent->children, child);

    free(parent->children);

    parent->children = temp;
  }
}

void appendProperty(struct Element *parent, char *property) {
  size_t propLen = strlen(property) + 1;
  if (parent->properties == NULL) {
    parent->properties = (char*)malloc(sizeof(char) * propLen);

    strncpy(parent->properties, property, propLen);
  } else {
    size_t newPropertiesLen = propLen + strlen(parent->properties) + 1;
    char *temp = (char*)malloc(sizeof(char) * newPropertiesLen);

    snprintf(temp, newPropertiesLen, "%s %s", parent->properties, property);

    free(parent->properties);

    parent->properties = temp;
  }
}

char *ElementToString(struct Element *element) {
  // TODO implement, this one should always return a pointer to a string,
  // as __ArielCloseBlock adds to the next lower stack element
  // Do not free either, also handled in __ArielCloseBlock

  size_t nameLen = strlen(element->name);
  if (element->properties == NULL) {
    element->properties = (char*)malloc(sizeof(char));
    //strncpy(element->properties, "", 1);
    *element->properties = 0;
  } else {
    size_t tempLen = strlen(element->properties) + 2;
    char *temp = (char*)malloc(sizeof(char) * tempLen);

    snprintf(temp, tempLen, " %s", element->properties);

    free(element->properties);
    element->properties = temp;
  }
  size_t propLen = strlen(element->properties);

  if (element->children == NULL) {
    element->children = (char*)malloc(sizeof(char));
    *element->children = 0;
  }
  size_t childLen = strlen(element->children);

  // name * 2, props, children, <></>, \0 = +6
  size_t outputLen = nameLen * 2 + propLen + childLen + 6;
  char *output = (char*)malloc(sizeof(char) * outputLen);

  snprintf(output, outputLen, "<%s%s>%s</%s>", element->name, element->properties, element->children, element->name);

  return output;
}

void freeElement(struct Element *element) {
  free(element->name);
  free(element->properties);
  free(element->children);
  free(element);
}

char *createProperty(char *name, char *value) {
  size_t nameLen = strlen(name);
  size_t valueLen = strlen(value);

  // +3 for ="", +1 for \0
  size_t outputLen = nameLen + valueLen + 3 + 1;

  char *output = (char*)malloc(sizeof(char) * outputLen);

  snprintf(output, outputLen, "%s=\"%s\"", name, value);

  return output;
}

char *createText(char *text) {
  size_t textLen = strlen(text) + 1;

  char *output = (char*)malloc(sizeof(char) * textLen);
  strncpy(output, text, textLen);

  return output;
}


#endif
