#ifndef FUZZYCONTROL_H
#define FUZZYCONTROL_H

class Fuzzy
{
  
  public:
    
    // Main function
    Fuzzy(const float alpha, const float alpha_v, unsigned int mainWait){
      _alpha = alpha;
      _alpha_v = alpha_v;
      _mainWait = mainWait;
    };


    /*
     * This function serves to filter data coming in from the Pixy. This function
     * uses an Exponential Moving Average (EMA) to filter out data from the pixy,
     * such as the x coordinates and the height of the blocks detected
    */
    void filterPixy(int newData){

      // init variables 
      int variable_sample = 0; 
      int variable_v_sample = 0;
      variable_avg = 0.0;
      variable_v_avg = 0.0;

      // filter data
      variable_sample = newData;
      
      variable_avg = (variable_sample * _alpha) + ((last_variable)*(1-_alpha)); // calc variable average
      variable_v_time = millis(); // update time
      variable_v_sample = ((variable_avg - last_variable)*100.0) / (variable_v_time - last_variable_v_time); // update sample
      variable_v_avg = (variable_v_sample * _alpha_v) + ((last_variable_v)*(1- _alpha_v)); // update average

      // update last variables
      last_variable = variable_avg;
      last_variable_v = variable_v_avg;
      last_variable_v_time = variable_v_time;

    }
   
    /*
    * This function serves to filter data coming in from the Pixy. This function
    * uses an Exponential Moving Average (EMA) to filter out data from the pixy,
    * such as the x coordinates and the height of the blocks detected
    */
    void updateMembership(){
      
      // update position variable membership
      memVariable[0] = tmfl(variable_avg, midVariable[0], midVariable[1]);
      memVariable[1] = tmf(variable_avg, midVariable[0], midVariable[1], midVariable[2]);
      memVariable[2] = tmf(variable_avg, midVariable[1], midVariable[2], midVariable[3]);
      memVariable[3] = tmf(variable_avg, midVariable[2], midVariable[3], midVariable[4]);
      memVariable[4] = tmfr(variable_avg, midVariable[3], midVariable[4]);

      // update velocity variable membership
      memVariableChange[0] = tmfl(variable_v_avg, midVariableChange[0], midVariableChange[1]);
      memVariableChange[1] = tmf(variable_v_avg, midVariableChange[0], midVariableChange[1], midVariableChange[2]);
      memVariableChange[2] = tmf(variable_v_avg, midVariableChange[1], midVariableChange[2], midVariableChange[3]);
      memVariableChange[3] = tmf(variable_v_avg, midVariableChange[2], midVariableChange[3], midVariableChange[4]);
      memVariableChange[4] = tmfr(variable_v_avg, midVariableChange[3], midVariableChange[4]);
      
    }

    int getCrispOutput(){
      int crispOutput = inference(p_degreeFiringVariable, p_memVariable, p_memVariableChange, p_rule); // send pointer of degreeFiringSteering to inference function
      // Serial.print(crispSteering);
      return crispOutput;
    }

    void setSingletons(int FF, int FS, int Z, int BS, int BF){
      _FF = FF;
      _FS = FS;
      _Z = Z;
      _BS = BS;
      _BF = BF;

      // Input sets
      int newRule[5][5] = {
        {_FF, _FF, _FF, _FS, _Z},
        {_FF, _FF, _FS, _Z, _BS},
        {_FF, _FS, _Z, _BS, _BF},
        {_FS, _Z, _BS, _BF, _BF},
        {_Z, _BS, _BF, _BF, _BF}
      };

      for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
          rule[i][j] = newRule[i][j];
        }
      }

      (*p_rule)[5][5] = &rule;
    
    }


    // This function sets the midpoints for the position function of the variable
    void setMidPoints(float first, float seconD, float third, float fourth, float fifth){
      midVariable[0] = first;
      midVariable[1] = seconD; // second was a variable already for something Arduino related, so seconD was used instead to not mess with the compiler
      midVariable[2] = third;
      midVariable[3] = fourth;
      midVariable[4] = fifth;
    }


    // This function sets the midpoints for the velocity function of the variable
    void setMidPointsV(float first, float seconD, float third, float fourth, float fifth){
      midVariableChange[0] = first;
      midVariableChange[1] = seconD; // second was a variable already for something Arduino related, so seconD was used instead to not mess with the compiler
      midVariableChange[2] = third;
      midVariableChange[3] = fourth;
      midVariableChange[4] = fifth;
    }

    // Crisp output for steering and throttle
    int crispOutput = 0;

    
  private:
    
    float _alpha = 0.5;
    float _alpha_v = 0.5;
    int _mainWait = 0;

    // EMA variables
    double variable_avg = 0.0;
    double variable_v_avg = 0.0;
    double variable_v_time = 0.0;
    double last_variable = 0.0;
    double last_variable_v = 0.0;
    double last_variable_v_time = 0.0;

    // Timers
    unsigned int mainTimer = 0;
    unsigned int mainWait = 30;
    unsigned int mainLast = 0;

    // Variable singletons
    int _FF = 1650;
    int _FS = 1550;
    int _Z = 1450;
    int _BS = 1350;
    int _BF = 1250;


    // Input sets
    int rule[5][5] = {
      {_FF, _FF, _FF, _FS, _Z},
      {_FF, _FF, _FS, _Z, _BS},
      {_FF, _FS, _Z, _BS, _BF},
      {_FS, _Z, _BS, _BF, _BF},
      {_Z, _BS, _BF, _BF, _BF}
    };

    int (*p_rule)[5][5] = &rule;

    // Mid points

    float midVariable[5] = {0,0,0,0,0};
    float midVariableChange[5] = {0,0,0,0,0};

    // Degree of membership 
    float memVariable[5] = {0,0,0,0,0};
    float (*p_memVariable)[5] = &memVariable;
    float memVariableChange[5] = {0,0,0,0,0};
    float (*p_memVariableChange)[5] = &memVariable;

    // Degree of Firing for Variable
    float degreeFiringVariable[5][5] = {
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},  
    };
    
    // Pointer for degreeFiringVariable array. Used for inference function
    float (*p_degreeFiringVariable)[5][5] = &degreeFiringVariable;

    float tmf(float inVal, float p1, float p2, float p3){ // tmf function for regular triangle
    
      float membership;
      
      if ((inVal > p1) && (inVal <= p2))
        membership = (1/(p2-p1))*inVal-p1/(p2-p1);
      else if ((inVal > p2) && (inVal < p3))
        membership = -(1/(p3-p2))*inVal+p3/(p3-p2);
      else
        membership = 0;
    
      return membership;
    }
  
    float tmfl(float inVal, float p1, float p2){ // tmf function left side
      float membership;
      if (inVal < p1)
        membership = 1;
      else if ((inVal < p1) && (inVal > p2))
        membership = -(1/(p2-p1))*inVal+p2/(p2-p1);
      else
        membership = 0;
      
      return membership;
    }
    
    float tmfr(float inVal, float p1, float p2){ // tmf function right side
      float membership;
      if (inVal > p2)
        membership = (1/(p2-p1))*inVal-p1/(p2-p1);
      else if ((inVal < p1) && (inVal > p2))
        membership = 1;
      else
        membership = 0;
    
      return membership;
    }
    
    // assigns degree to each rule in rule base for the height, then returns crisp output
    int inference(float (*two_d_array)[5][5], float (*mem_two_d_array)[5], float (*mem_vel_two_d_array)[5], int (*rule_matrix)[5][5]){  // since in c++ you cannot pass a double array, pass a pointer to the array
      float numerator = 0.0;
      float denominator = 0.0;
      for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
          (*two_d_array)[i][j] = (*mem_two_d_array)[i] * (*mem_vel_two_d_array)[j];
          numerator += (*two_d_array)[i][j] * (*rule_matrix)[i][j]; // using same rule set, so no need to pass it in as a pointer
          denominator += (*two_d_array)[i][j];
        }
      }
      
      return (int) (numerator / denominator);
    
    }

    
    
};

#endif
