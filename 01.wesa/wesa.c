#include "ext.h"
#include "ext_obex.h"

typedef struct {
  t_object m_obj;
  int m_output;
  void** m_outlets;
  void** m_inlets;
} t_wesa;

static t_class* wesa_class;

void* wesa_new(long numoutputs, long output) {
  t_wesa* x = (t_wesa*)object_alloc(wesa_class);

  if (output >= numoutputs) {
    object_error((t_object*)x, "too many output");
    return NULL;
  }

  x->m_inlets = (void**)malloc(sizeof(void*) * 4);

  for(int i = 3; i >=0; i--) {
    x->m_inlets[i] = proxy_new(x, i + 1, 0);
  }
  
  x->m_output = output;
  x->m_outlets = (void**)malloc(sizeof(void*) * numoutputs);
  
  for(int i = numoutputs - 1 ; i >= 0; i--) {
    x->m_outlets[i] = outlet_new(x, NULL);
  }
  
  return x;
}

void wesa_free(t_wesa* x) {
  free(x->m_outlets);
  for(int i = 0; i < 4; i++) {
    object_free(x->m_inlets[i]);
  }
  free(x->m_inlets);
}

void wesa_float(t_wesa* x, double value) {
  int inlet =  proxy_getinlet((t_object*)x);
  object_post((t_object*)x, "data from inlet: %d", inlet);
  if(inlet == 0) {
    outlet_float(x->m_outlets[x->m_output], value * 100);
  }
  else if (inlet == 1) {
    outlet_float(x->m_outlets[x->m_output], value * -100);
  }
  else {
    outlet_float(x->m_outlets[0], value);
  }
}

void wesa_int(t_wesa* x, long value) {
  outlet_int(x->m_outlets[x->m_output], value + 100);
}

void wesa_bang(t_wesa* x) {
  outlet_bang(x->m_outlets[x->m_output]);
}

void wesa_anything(t_wesa* x, t_symbol* s, long argc, t_atom* v) {
  object_error((t_object*)x, "wesa can't understand message: %s with %d arguments", s->s_name, argc);
}

void wesa_render(t_wesa* x, double a, double b) {
  object_post((t_object*)x, "wesa render %f", a + b);
}

void wesa_foo(t_wesa* x) {
  object_post((t_object*)x, "wesa foo!");
}

void wesa_symbol(t_wesa* x, t_symbol* s) {
  object_post((t_object*)x, "receive symbol %s", s->s_name);
}

void wesa_list(t_wesa* x, t_symbol* s, long argc, t_atom* v) {
  t_atom a[argc];
  for(int i = 0; i < argc; i++) {
    atom_setlong(a+i, i + 100);
  }
  outlet_list(x->m_outlets[x->m_output], NULL, argc, a);
}

void ext_main(void* r) {
  post("welcome to wesa!");
  //                                   생성자             소멸자         인스턴스의 크기   NULL
  wesa_class = class_new("wesa", (method)wesa_new, (method)wesa_free, sizeof(t_wesa), NULL, A_LONG, A_LONG, 0);
  class_addmethod(wesa_class, (method)wesa_float, "float", A_FLOAT, 0);
  class_addmethod(wesa_class, (method)wesa_int, "int", A_LONG, 0);
  class_addmethod(wesa_class, (method)wesa_bang, "bang", 0);
  class_addmethod(wesa_class, (method)wesa_symbol, "symbol", A_SYM, 0);
  class_addmethod(wesa_class, (method)wesa_list, "list", A_GIMME, 0);
  class_addmethod(wesa_class, (method)wesa_render, "render", A_FLOAT, A_FLOAT, 0);
  class_addmethod(wesa_class, (method)wesa_foo, "foo", 0);
  class_addmethod(wesa_class, (method)wesa_anything, "anything", A_GIMME, 0);
  
  class_register(CLASS_BOX, wesa_class);
}

