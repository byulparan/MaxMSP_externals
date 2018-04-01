#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#undef PI
#include "JCRev.h"

using namespace stk;

typedef struct {
  t_pxobject m_obj;
  JCRev* m_reverb;
  float m_samplerate;
} t_wesa_stk_jcrev;

static t_class* wesa_stk_jcrev_class;

void* wesa_stk_jcrev_new() {
  t_wesa_stk_jcrev* x = (t_wesa_stk_jcrev*)object_alloc(wesa_stk_jcrev_class);
  
  x->m_reverb = NULL;
  x->m_samplerate = 0;
  
  dsp_setup((t_pxobject*)x, 1);

  outlet_new(x, "signal");
  outlet_new(x, "signal");

  return x;
}

void wesa_stk_jcrev_free(t_wesa_stk_jcrev* x) {
  dsp_free((t_pxobject*)x);
}

void wesa_stk_jcrev_perform(t_wesa_stk_jcrev* x, t_object* dsp, double** ins, long numins, double** outs, long numouts, long sampleframe, long flags, void* userdata) {

  for(int i = 0; i < sampleframe; i++) {
    double input = ins[0][i];
    outs[0][i] = x->m_reverb->tick(input, 0);
    outs[1][i] = x->m_reverb->tick(input, 1);
  }
  
}

void wesa_stk_jcrev_dsp(t_wesa_stk_jcrev* x, t_object* dsp, short* count, double samplerate, long vecsize, long flags) {

  if (x->m_reverb == NULL || x->m_samplerate != samplerate) {
    if (x->m_reverb != NULL) {
      delete x->m_reverb;
    }
    
    Stk::setSampleRate( samplerate );
    x->m_samplerate = samplerate;
    x->m_reverb = new JCRev();
  }
  
  object_method(dsp, gensym("dsp_add64"), x, wesa_stk_jcrev_perform, 0, NULL);
}


void wesa_stk_jcrev_revtime(t_wesa_stk_jcrev* x, double revtime) {

  if(x->m_reverb != NULL) {
    x->m_reverb->setT60(revtime);
  }
  
}


void ext_main(void* r) {
  wesa_stk_jcrev_class = class_new("wesa_stk_jcrev~", (method)wesa_stk_jcrev_new, (method)wesa_stk_jcrev_free, sizeof(t_wesa_stk_jcrev), NULL, 0);

  class_addmethod(wesa_stk_jcrev_class, (method)wesa_stk_jcrev_revtime, "revtime", A_FLOAT, 0);
  class_addmethod(wesa_stk_jcrev_class, (method)wesa_stk_jcrev_dsp, "dsp64", A_CANT, 0);
  class_dspinit(wesa_stk_jcrev_class);
  class_register(CLASS_BOX, wesa_stk_jcrev_class);
}

