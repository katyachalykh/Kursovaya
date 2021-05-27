#pragma once
#include <iostream>
#include <array>

#include "tim2registers.hpp"
#include "IColorChannel.h"
#include "nvicregisters.hpp"
#include "rccregisters.hpp"



constexpr float timerClock = 0.0000000625f;

class Timer2
{

 public:
 
      
 void HandleInterrupt()
 {
    //��������� ��� ������ �������� + ��������� ���������� �� ������� 
   if (TIM2::CCER::CC1E::Value1::IsSet() && TIM2::DIER::CC1IE::Enable::IsSet())
    {
       float currentvalue = TIM2::CCR1::Get(); // �������� �������� �� ������� � ������������ ����� ����������
       
       period = ((currentvalue - previouse)/4.0f)*timerClock;  // �����, ��� �������� ��������
       
       pCurrentChannel -> OnInterrupt(period);     
       previouse = currentvalue;
    }
 }
 
 void SetCurrentChannel(IColorChannel* currentChannel)
 {
    pCurrentChannel = currentChannel;
    pCurrentChannel-> SwitchOn();           
 }
 
 
  void Start()
 {
    TIM2::CCER::CC1E::Value1::Set(); // ������ ��������
    TIM2::DIER::CC1IE::Enable::Set(); // ���������� ���������
    HandleInterrupt();
 }
 
  void Stop()
 {
    TIM2::CCER::CC1E::Value0::Set(); // ������ ��������
    TIM2::DIER::CC1IE::Disable::Set(); // ���������� ���������
 }
 
 private:
  IColorChannel* pCurrentChannel;
  
  float previouse = 0.0f;
  float period = 0.0f;
 
};

inline Timer2 timer2;