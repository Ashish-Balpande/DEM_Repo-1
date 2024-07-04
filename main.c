#include <stdio.h>
#include <inttypes.h>

uint8_t pass_event = 0;
uint8_t fail_event = 0;

uint8_t status_mask[8] = {0,0,0,0,0,0,0,0};

int16_t debouncing_counter = 0;
int16_t Fault_Detect_Counter = 0;
int16_t Failure_counter = 0;
int16_t Ageing_counter = 0;
int16_t Healing_counter = 0;
int16_t Occurence_counter = 0;

uint8_t fc_threshold = 2;
uint8_t Ag_threshold = 3;
uint8_t He_threshold = 3;
uint8_t Oc_threshold = 3;
uint8_t def_threshold = 6;       //Event
uint8_t step_size = 50;
uint8_t event = 0;
uint8_t jumpUp_Down = 0;

uint8_t num=6, op_cy=1;         //Operational cycle

void main()
{

    while(num>0)
    {
        printf("\n******************* Operation Cycle: %d *******************\n", op_cy);
        start_project();
        op_cy++;
        num--;
    }
}

void start_project()
{
    check_FailureCounter();
    if(fc_threshold == Failure_counter)
        {
            status_mask[3] = 1;
            status_mask[7] = 1;
        }
    if(status_mask[3] == 1)
        {
            check_AgeingCounter();
            //printf("\nAgeing Counter: %d\n", Ageing_counter);
        }
    if(status_mask[7] == 1)
        {
            check_HealingCounter();
        }
    if(Ag_threshold == Ageing_counter)
        {
            status_mask[3] = 0;
        }
    if(He_threshold == Healing_counter)
        {
            status_mask[7] = 0;
        }

    display_Status_Mask();

    def_threshold = 6;              //event threshold

    while(def_threshold > 0)
    {
        printf("\nEnter the Event(1ms Task): Failed: 1 or Passed: 0 => ");
        scanf("%d", &event);

        check_DebouncingCounter();
        check_FDC();
        display_Status_Mask();

        def_threshold--;
    }

}

uint8_t add_events()
{
    uint8_t event = 0;
    printf("\n\n---------------------------------\nEnter the Event(1ms Task): Failed: 1 or Passed: 0\n");
    scanf("%d", &event);

    return event;
}

void check_DebouncingCounter()
{
    if(event == 1)  // fail
    {
        if(debouncing_counter < -128)
        {
            debouncing_counter = jumpUp_Down;
        }

        if(Fault_Detect_Counter < 127)
        {
            debouncing_counter += step_size;
            Occurence_counter++;
            check_Occurence_Counter();
        }
        status_mask[0] = 1;
    }
    else   //pass
    {
        if(debouncing_counter > 127)
        {
            debouncing_counter = jumpUp_Down;
        }

        if(Fault_Detect_Counter > -127)
        {
            debouncing_counter -= step_size;
        }
        status_mask[0] = 0;
    }
}

void check_FDC()
{
    printf("De-bouncing Counter: %d\n", debouncing_counter);
    if(debouncing_counter >= 127)
    {
        Fault_Detect_Counter = 127;
        status_mask[1] = 1;
        //check_FailureCounter();
        printf("FDC: Failed State: %d\n", Fault_Detect_Counter);

    }
    else if(debouncing_counter < 127 && debouncing_counter >=0)
    {
        Fault_Detect_Counter = 1;
        printf("FDC: Pre-Failed State: %d\n", Fault_Detect_Counter);
    }
    else if(debouncing_counter > (-128) && debouncing_counter <0)
    {
        Fault_Detect_Counter = -1;
        printf("FDC: Pre-Passed State: %d\n", Fault_Detect_Counter);
    }
    else
    {
        Fault_Detect_Counter = -128;
        printf("FDC: Passed State: %d\n", Fault_Detect_Counter);
    }
}

void check_FailureCounter()
{
    if(Fault_Detect_Counter >= 127)
    //if(status_mask[1] == 1)
    {
        Failure_counter++;
    }
    printf("\nFailure Counter: %d\n", Failure_counter);
}

void check_AgeingCounter()
{
    if(Fault_Detect_Counter <= -128)
    {
        Ageing_counter++;
    }
    printf("\Ageing Counter: %d\n", Ageing_counter);

}

void check_HealingCounter()
{
    if(Fault_Detect_Counter <= -128)
    {
        Healing_counter++;
    }
    printf("\Healing Counter: %d\n", Healing_counter);

}

void check_Occurence_Counter()
{
    if(Occurence_counter == 2)
    {
      status_mask[2] = 1;
    }
}

void display_Status_Mask()
{
    printf("\n---------------------------------------------------------\n");
    printf("Status Mask: \t\t\t");
    for(int i=0; i<8; i++)
    {
        printf("%d  ",status_mask[i]);
    }
    printf("\n---------------------------------------------------------\n");
}

