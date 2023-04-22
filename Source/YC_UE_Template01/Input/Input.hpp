#pragma once
#include <YC/ErrorOr/ErrorOr.hpp>

#include "EnhancedPlayerInput.h"
#include "InputActionValue.h"

class UInputAction;

static ErrorOr<FInputActionValue> InputCheck(const UEnhancedPlayerInput* InputComp, UInputAction* InInputAct) {
	if(!IsValid(InputComp)) return Err { std::string("InputComp is not valid") };
	return InputComp->GetActionValue(InInputAct);
}
