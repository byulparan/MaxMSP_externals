#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#undef PI
#include "Saxofony.h"

using namespace stk;

typedef struct {
  t_pxobject m_obj;
  Saxofony* m_saxo;
  float m_samplerate;
} t_wesa_stk_saxo;

static t_class* wesa_stk_saxo_class;

void* wesa_stk_saxo_new() {
  t_wesa_stk_saxo* x = (t_wesa_stk_saxo*)object_alloc(wesa_stk_saxo_class);
  
  x->m_saxo = NULL;
  x->m_samplerate = 0;
  
  dsp_setup((t_pxobject*)x, 0);

  outlet_new(x, "signal");

  return x;
}

void wesa_stk_saxo_free(t_wesa_stk_saxo* x) {
  dsp_free((t_pxobject*)x);
}

void wesa_stk_saxo_perform(t_wesa_stk_saxo* x, t_object* dsp, double** ins, long numins, double** outs, long numouts, long sampleframe, long flags, void* userdata) {

  for(int i = 0; i < sampleframe; i++) {
    outs[0][i] = x->m_saxo->tick();
  }
}

void wesa_stk_saxo_dsp(t_wesa_stk_saxo* x, t_object* dsp, short* count, double samplerate, long vecsize, long flags) {

  if (x->m_saxo == NULL || x->m_samplerate != samplerate) {
    Stk::setSampleRate( samplerate );

    if (x->m_saxo != NULL) {
      delete x->m_saxo;
    }
    x->m_samplerate = samplerate;
    x->m_saxo = new Saxofony(100);
  }
  
  object_method(dsp, gensym("dsp_add64"), x, wesa_stk_saxo_perform, 0, NULL);
}

void wesa_stk_saxo_note_on(t_wesa_stk_saxo* x, double freq, double amp) {

  if (x->m_saxo != NULL) {
    x->m_saxo->noteOn(freq, amp);
  }
}

void wesa_stk_saxo_note_off(t_wesa_stk_saxo* x, double amp) {
  if (x->m_saxo != NULL) {
    x->m_saxo->noteOff(amp);
  }
}



void ext_main(void* r) {
  wesa_stk_saxo_class = class_new("wesa_stk_saxo~", (method)wesa_stk_saxo_new, (method)wesa_stk_saxo_free, sizeof(t_wesa_stk_saxo), NULL, 0);

  class_addmethod(wesa_stk_saxo_class, (method)wesa_stk_saxo_dsp, "dsp64", A_CANT, 0);
  class_addmethod(wesa_stk_saxo_class, (method)wesa_stk_saxo_note_on, "note_on", A_FLOAT, A_FLOAT, 0);
  class_addmethod(wesa_stk_saxo_class, (method)wesa_stk_saxo_note_off, "note_off", A_FLOAT, 0);

  
  class_dspinit(wesa_stk_saxo_class);
  class_register(CLASS_BOX, wesa_stk_saxo_class);
}

