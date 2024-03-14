// Fill out your copyright notice in the Description page of Project Settings.


#include "PostLoadAsset.h"

FPrimaryAssetId UPostLoadAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("UPostLoadAsset", GetFName());
}
