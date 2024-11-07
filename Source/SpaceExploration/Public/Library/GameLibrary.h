// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ゲームで使用する関数、型の共有ライブラリ
 * 
 */

// 各の工程を制御するためのデリゲート
DECLARE_DELEGATE_RetVal_OneParam(bool, Sequence, const float);