#ifndef DART_SENSOR_H
#define DART_SENSOR_H

#include <stdint.h>
#include <SoftwareSerial.h>

class DartSensor
{
public:
  DartSensor(int rx, int tx);

  void init();
  int32_t fetch_value(); 
private:
  bool checksum(byte *frame);
private:
  static const size_t FRAME_LENGTH = 9;
private:
  SoftwareSerial  serial_;
  uint32_t        baud_;
  bool            working_;
};

DartSensor::DartSensor(int rx, int tx) : 
    serial_(rx, tx),
    baud_(9600), 
    working_(false)
{
  
}

void DartSensor::init()
{
  serial_.begin(baud_);
}

int32_t DartSensor::fetch_value()
{
  int32_t r = -1;
  if (working_)
  {
    while (true)
    {
      size_t size = serial_.available();
      if (size >= FRAME_LENGTH)
      {
        byte frame[FRAME_LENGTH];
        for (size_t i = 0; i < FRAME_LENGTH; ++i)
        {
          frame[i] = serial_.read();
        }
        if (checksum(frame))
        {
          r = frame[4] * 256 + frame[5];
        }
        else
        {
          working_ = false;
          Serial.println("Failed to checksum. Stop working");
          break;
        }
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    size_t size = serial_.available();
    Serial.print("read:");
    Serial.println(size);
    while (size > 0)
    {
      byte b = serial_.peek();
      if (b == 0xFF)
      {
        working_ = true;
        Serial.println("DartSensor is ready to work.");
        break;
      }
      else
      {
        serial_.read(); // skip
        --size;
      }
    }
  }
  return r;
}

bool DartSensor::checksum(byte *frame)
{
  byte t = 0;
  for (size_t i = 1; i < FRAME_LENGTH - 1; ++i)
  {
    t += frame[i];
  }
  t = ~t + 1;
  return t == frame[8];
}

#endif
