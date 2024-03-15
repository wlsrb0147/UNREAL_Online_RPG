// Fill out your copyright notice in the Description page of Project Settings.


#include "PostLoadAsset.h"

FPrimaryAssetId UPostLoadAsset::GetPrimaryAssetId() const
{
	UE_LOG(LogTemp, Error, TEXT("8888888888888888899900078 : %s"), *GetFName().ToString());
	return FPrimaryAssetId("PostLoadAsset", GetFName());

}
