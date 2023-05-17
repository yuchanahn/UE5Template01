#pragma once
#include <YC/ErrorOr/ErrorOr.hpp>

#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"

class UInputAction;

static ErrorOr<FInputActionValue> InputCheck_(UInputAction* InInputAct, UEnhancedPlayerInput* InputComp) {
	if(!IsValid(InInputAct)) return Err { std::string("InInputAct is not valid") };
	if(!IsValid(InputComp)) return Err { std::string("InputComp is not valid") };
	return InputComp->GetActionValue(InInputAct);
}
//InputCheck_(UInputAction*, UEnhancedPlayerInput*) -> ErrorOr<FInputActionValue>
static auto InputCheck = Curry(InputCheck_);