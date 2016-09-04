#ifndef FIELDEMISSIONBC_H
#define FIELDEMISSIONBC_H

#include "IntegratedBC.h"

class FieldEmissionBC;

template<>
InputParameters validParams<FieldEmissionBC>();

class FieldEmissionBC : public IntegratedBC
{
public:

	FieldEmissionBC(const InputParameters & parameters);

protected:
	virtual Real computeQpResidual();
	virtual Real computeQpJacobian();
	virtual Real computeQpOffDiagJacobian(unsigned int jvar);

	Real _r_units;
	Real _r;

	// Coupled variables

	const VariableGradient & _grad_potential;
	unsigned int _potential_id;

	Real _e;
	Real _work_function;
	Real _field_enhancement;
};

#endif //FIELDEMISSIONBC_H
