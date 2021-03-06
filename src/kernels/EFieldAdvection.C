//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EFieldAdvection.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("ZapdosApp", EFieldAdvection);

template <>
InputParameters
validParams<EFieldAdvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar(
      "potential", "The gradient of the potential will be used to compute the advection velocity.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addClassDescription("Generic electric field driven advection term"
                             "(Densities must be in log form)");
  return params;
}

EFieldAdvection::EFieldAdvection(const InputParameters & parameters)
  : Kernel(parameters),

    _r_units(1. / getParam<Real>("position_units")),

    _mu(getMaterialProperty<Real>("mu" + _var.name())),
    _sign(getMaterialProperty<Real>("sgn" + _var.name())),

    // Coupled variables

    _potential_id(coupled("potential")),
    _grad_potential(coupledGradient("potential"))
{
}

Real
EFieldAdvection::computeQpResidual()
{
  return _mu[_qp] * _sign[_qp] * std::exp(_u[_qp]) * -_grad_potential[_qp] * _r_units *
         -_grad_test[_i][_qp] * _r_units;
}

Real
EFieldAdvection::computeQpJacobian()
{
  return _mu[_qp] * _sign[_qp] * std::exp(_u[_qp]) * _phi[_j][_qp] * -_grad_potential[_qp] *
         _r_units * -_grad_test[_i][_qp] * _r_units;
}

Real
EFieldAdvection::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _potential_id)
    return _mu[_qp] * _sign[_qp] * std::exp(_u[_qp]) * -_grad_phi[_j][_qp] * _r_units *
           -_grad_test[_i][_qp] * _r_units;
  else
    return 0.;
}
