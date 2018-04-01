#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"

#include <random>
#include <cmath>

typedef struct {
  t_pxobject m_obj;
  float m_samplerate;
  float m_phase;
  float m_incr;
} t_wesa;

static t_class* wesa_class;

void* wesa_new() {
  t_wesa* x = (t_wesa*)object_alloc(wesa_class);
  
  dsp_setup((t_pxobject*)x, 2);

  for(int i = 0; i < 2; i++) {
    outlet_new(x, "signal");
  }

  x->m_phase = 0.0;

  return x;
}

void wesa_free(t_wesa* x) {
  dsp_free((t_pxobject*)x);
}

void wesa_perform(t_wesa* x, t_object* dsp, double** ins, long numins, double** outs, long numouts, long sampleframe, long flags, void* userdata) {

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_real_distribution<> random(-1.0, 1.0);

  for(int i = 0; i < sampleframe; i++) {
    outs[0][i] = random(gen);
    outs[1][i] = sin(x->m_phase);
    x->m_phase = std::fmod(x->m_phase + x->m_incr, 2 * PI);
  }
  
}

void wesa_dsp(t_wesa* x, t_object* dsp, short* count, double samplerate, long vecsize, long flags) {

  x->m_incr = (440 * 2 * PI / samplerate);
  x->m_samplerate = samplerate;
  object_method(dsp, gensym("dsp_add64"), x, wesa_perform, 0, NULL);
}

void wesa_float(t_wesa* x, double freq) {
  if (proxy_getinlet((t_object*)x) == 0) {
    x->m_incr = (freq * 2 * PI / x->m_samplerate);
  }
}


void ext_main(void* r) {
  wesa_class = class_new("wesa~", (method)wesa_new, (method)wesa_free, sizeof(t_wesa), NULL, 0);

  class_addmethod(wesa_class, (method)wesa_dsp, "dsp64", A_CANT, 0);
  class_addmethod(wesa_class, (method)wesa_float, "float", A_FLOAT, 0);
  
  class_dspinit(wesa_class);
  class_register(CLASS_BOX, wesa_class);
}

