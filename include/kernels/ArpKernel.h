#ifndef ARPKERNEL_H
#define ARPKERNEL_H

#include "Kernel.h"

class ArpKernel;

template<>
InputParameters validParams<ArpKernel>();

class ArpKernel : public Kernel
{
 public:
  ArpKernel(const std::string & name, InputParameters parameters);
  virtual ~ArpKernel();

 protected:
  
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // coupled vars

  VariableValue & _em;
  VariableGradient & _grad_em;
  VariableGradient & _grad_potential;
  unsigned int _potential_id;
  VariableValue & _mean_en;

  // Kernel members

  Real _muem;
  Real _diff;
  Real _k4_const;
  Real _Ar;
  Real _Eiz;
  Real _muArp;
  Real _DArp;
  Real _a0;
  Real _b0;
  Real _c0;
};

#endif /* ARPKERNEL_H */
