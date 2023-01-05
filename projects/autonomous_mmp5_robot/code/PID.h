#ifndef PID_H
#define PID_H
#define DEADBAND 30

class PID 
{
    public:
        PID(int32_t propGain, int32_t integralGain, int32_t derivativeGain){
            _propGain = propGain;
            _integralGain = integralGain;
            _derivativeGain = derivativeGain;

            motor_signal = 0;
            _prev_Error = 0x80000000L;
        };

        void update(int error){
            int32_t pid;

            if (_prev_Error != 0x80000000L){
              pid = (error * _propGain);
              
              // Deal with deadband
              if (pid>0){
                pid += DEADBAND;
              } else if (pid<0) {
                pid -= DEADBAND;
              } 
                motor_signal = (int32_t) pid;
            }
            _prev_Error = error; // Update memory for derivative term
        }
        
        int32_t motor_signal;
    private:
        int32_t _propGain; // Proportional gain for PID
        int32_t _integralGain; // Integral gain for PID
        int32_t _derivativeGain; // Derivative gain for PID
        int32_t _prev_Error; // Previous error
        int32_t _sumIntegral; // Sum of integral error
};
    



#endif
