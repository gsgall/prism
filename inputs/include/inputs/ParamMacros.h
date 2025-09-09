//* This file is a part of PRISM: Plasma Reaction Input SysteM,
//* A library for parcing chemical reaction networks for plasma chemistry
//* https://github.com/NCSU-ComPS-Group/prism
//*
//* Licensed under MIT, please see LICENSE for details
//* https://opensource.org/license/mit
//*
//* Copyright 2024, North Carolina State University
//* ALL RIGHTS RESERVED

#define declareParam(name, default, description, params, type)                                     \
  params.addParam<type>(name, default, description, __FILE__, __FUNCTION__, __LINE__)

#define declareCheckedParam(name, default, description, validator, params, type)                   \
  params.addParam<type>(name, default, description, validator, __FILE__, __FUNCTION__, __LINE__)

#define declareRequiredParam(name, description, params, type)                                      \
  params.addRequiredParam<type>(name, description, __FILE__, __FUNCTION__, __LINE__)

#define declareRequiredCheckedParam(name, description, validator, params, type)                    \
  params.addRequiredParam<type>(name, description, validator, __FILE__, __FUNCTION__, __LINE__)

#define declareRepeatedTypedBlock(name, type, block_params, params)                                \
  params.addRepeatedTypedBlock(name, type, block_params, __FILE__, __FUNCTION__, __LINE__)

#define declareRequiredRepeatedTypedBlock(name, type, block_params, params)                        \
  params.addRequiredRepeatedTypedBlock(name, type, block_params, __FILE__, __FUNCTION__, __LINE__)

#define declareRepeatedBlock(name, block_params, params)                                           \
  params.addRepeatedBlock(name, block_params, __FILE__, __FUNCTION__, __LINE__)

#define declareRequiredRepeatedBlock(name, block_params, params)                                   \
  params.addRequiredRepeatedBlock(name, block_params, __FILE__, __FUNCTION__, __LINE__)
