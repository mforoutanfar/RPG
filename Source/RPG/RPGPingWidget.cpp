// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPingWidget.h"
#include "Components/Image.h"

void URPGPingWidget::ChangeToDefault()
{
	PingImage->SetBrushFromTexture(DefaultTexture);
}

void URPGPingWidget::ChangeToPointer()
{
	PingImage->SetBrushFromTexture(PointerTexture);
}
