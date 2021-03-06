//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CoeffDiffusionElectrons.h"

registerMooseObject("ZapdosApp", CoeffDiffusionElectrons);

template <>
InputParameters
validParams<CoeffDiffusionElectrons>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("mean_en",
                               "The log of the product of mean energy times electron density.");
  params.addRequiredParam<Real>("position_units", "Units of position");
  params.addClassDescription("Electron specific diffusion term"
                             "(Electron density must be in log form)");
  return params;
}

// This diffusion kernel should only be used with species whose values are in the logarithmic form.

CoeffDiffusionElectrons::CoeffDiffusionElectrons(const InputParameters & parameters)
  : Kernel(parameters),

    _r_units(1. / getParam<Real>("position_units")),

    _diffem(getMaterialProperty<Real>("diffem")),
    _d_diffem_d_actual_mean_en(getMaterialProperty<Real>("d_diffem_d_actual_mean_en")),

    _mean_en(coupledValue("mean_en")),
    _mean_en_id(coupled("mean_en")),

    _d_diffem_d_u(0),
    _d_diffem_d_mean_en(0)
{
}

CoeffDiffusionElectrons::~CoeffDiffusionElectrons() {}

Real
CoeffDiffusionElectrons::computeQpResidual()
{
  return -_diffem[_qp] * std::exp(_u[_qp]) * _grad_u[_qp] * _r_units * -_grad_test[_i][_qp] *
         _r_units;
}

Real
CoeffDiffusionElectrons::computeQpJacobian()
{
  _d_diffem_d_u =
      _d_diffem_d_actual_mean_en[_qp] * std::exp(_mean_en[_qp] - _u[_qp]) * -_phi[_j][_qp];

  return -_diffem[_qp] *
             (std::exp(_u[_qp]) * _grad_phi[_j][_qp] * _r_units +
              std::exp(_u[_qp]) * _phi[_j][_qp] * _grad_u[_qp] * _r_units) *
             -_grad_test[_i][_qp] * _r_units -
         _d_diffem_d_u * std::exp(_u[_qp]) * _grad_u[_qp] * _r_units * -_grad_test[_i][_qp] *
             _r_units;
}

Real
CoeffDiffusionElectrons::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _mean_en_id)
  {
    _d_diffem_d_mean_en =
        _d_diffem_d_actual_mean_en[_qp] * std::exp(_mean_en[_qp] - _u[_qp]) * _phi[_j][_qp];

    return -_d_diffem_d_mean_en * std::exp(_u[_qp]) * _grad_u[_qp] * _r_units *
           -_grad_test[_i][_qp] * _r_units;
  }

  else
    return 0.;
}
