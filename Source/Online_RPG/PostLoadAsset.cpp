// Fill out your copyright notice in the Description page of Project Settings.


#include "PostLoadAsset.h"

#include "WorldPartition/ContentBundle/ContentBundleLog.h"

FPrimaryAssetId UPostLoadAsset::GetPrimaryAssetId() const
{
	UE_LOG(LogTemp, Log, TEXT(" 77777777777777   init 7777777778777777777 "));
	return FPrimaryAssetId("PostLoadAsset", GetFName());
}
