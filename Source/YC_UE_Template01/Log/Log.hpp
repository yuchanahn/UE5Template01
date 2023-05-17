#pragma once

namespace YC {
struct Log {
	static void Push(const FString& Msg) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

	static void Push_Dp(const FString& Msg, const float Duration = 5.f, const FColor& Color = FColor::Red) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Msg);
	}

	static void Push_S(const std::string& Msg) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(UTF8_TO_TCHAR(Msg.c_str())));
	}

	static void Push_Dp_S(const std::string& Msg, const float Duration = 5.f, const FColor& Color = FColor::Red) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, *FString(UTF8_TO_TCHAR(Msg.c_str())));
	}
};
}
