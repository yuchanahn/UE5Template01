#pragma once
#include <YC/ErrorOr/ErrorOr.hpp>

#include "Http.h"

namespace YC::Http::Spreadsheets {

inline ErrorOr<void> Get(
	const UObject* G,
	const FString& GoogleSheetId,
	const FString& SheetName,
	std::function<void(ErrorOr<FString>)> Callback) {
	// HttpModule 인스턴스 가져오기
	FHttpModule& HttpModule = FHttpModule::Get();
	
	// HTTP 요청 생성
	const TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(FString::Printf(L"https://docs.google.com/spreadsheets/d/%s/export?sheet=%s&format=csv", *GoogleSheetId, *SheetName));
	
	// HTTP 응답 콜백 설정
	HttpRequest->OnProcessRequestComplete().BindWeakLambda(G,
		[Callback](FHttpRequestPtr Request, const FHttpResponsePtr& Response, const bool bWasSuccessful) {
			ErrorOr<FString> Result = Err { std::string("HttpRequest->ProcessRequest() failed") };
			if (bWasSuccessful) {
				Result = Response->GetContentAsString();
			}
			Callback(Result);
		});
	
	// HTTP 요청 보내기
	return HttpRequest->ProcessRequest() ? ErrorOr<void> {}
										 : Err { std::string("HttpRequest->ProcessRequest() failed") };
}

}
