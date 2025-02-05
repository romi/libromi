/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */
#include "block.h"
#include "stepper.h"
#include "config.h"
#include <ArduinoSerial.h>
#include <RomiSerial.h>

using namespace romiserial;
/**
 *  @brief The possible states of the controller (the main thread).
 */
enum {
        STATE_RUNNING = 'r',
        STATE_PAUSED = 'p',
        STATE_HOMING = 'h',
        STATE_ERROR = 'e'
};

extern volatile block_t *current_block;

uint8_t controller_state;

static const char *kInvalidState = "Invalid state";

// Function declarations for message handling
void handle_moveto(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_move(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_pause(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_continue(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_reset(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_zero(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_position(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_idle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_set_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_spindle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_test(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);

// Message handler configuration
const static MessageHandler handlers[] = {
        { 'm', 4, false, handle_moveto },
        { 'M', 4, false, handle_move },
        { 'V', 3, false, handle_moveat },
        { 'p', 0, false, handle_pause },
        { 'c', 0, false, handle_continue },
        { 'r', 0, false, handle_reset },
        { 'z', 0, false, handle_zero },
        { 'P', 0, false, send_position },
        { 'I', 0, false, send_idle },
        { 'H', 0, false, handle_homing },
        { 'h', 3, false, handle_set_homing },
        { 'E', 1, false, handle_enable },
        { 'S', 1, false, handle_spindle },
        { 'T', 1, false, handle_test },
        { '?', 0, false, send_info },
};

ArduinoSerial serial(Serial);  // Wrapper for Arduino's Serial interface
RomiSerial romiSerial(serial, serial, handlers, sizeof(handlers) / sizeof(MessageHandler));  // Main communication interface

static char reply_string[80];  // Buffer for serial replies
static int8_t homing_axes[3] =  {-1, -1, -1};  // Homing configuration for each axis
static int16_t homing_speeds[3] =  {1000, 1000, 400};  // Speeds during homing operations
static uint8_t limit_switches[3] = {0, 0, 0};  // Current state of the limit switches
static uint8_t old_z = 0;  // Tracks change in Z limit state

int moveat(int dx, int dy, int dz);


/**
 * @brief Clears movement buffer and resets stepper motors.
 */
void reset()
{
        block_buffer_clear();
        stepper_reset();
}

/**
 * @brief Check the accuracy of the path following.
 *
 * Check that the difference between the actual position of the arm,
 * as measured by the encoders, and the supposed position of the arm,
 * as measured by the number of executed motor steps, is less than a
 * given threshold. If the deviation is larger than the threshold then
 * the controller will stop the execution and request the controlling
 * program to make the necessary adjustments.
 */
void check_accuracy()
{
}

/**
 * @brief Initializes the system resources and components.
 */
void setup()
{
        // disable_driver();  // Disables the stepper driver for safety
        
        Serial.begin(115200);
        while (!Serial);  // Waits for Serial connection to initialize
        
        init_block_buffer();
        init_pins();
        init_stepper();

        controller_state = STATE_RUNNING;  // Sets the initial controller state
        enable_stepper_timer();
        romiSerial.send("Init OK");  // Sends initialization confirmation

}

static unsigned long last_time = 0;  // Last recorded time for periodic operations
static unsigned long last_print_time = 0;  // Time for printing diagnostics
static int16_t id = 0;  // ID tracker for commands

/**
 * @brief The main execution loop of the controller.
 */
void loop()
{
        update_limit_switches();
        romiSerial.handle_input();  // Handles any received serial commands
        check_accuracy();  // Periodically checks system accuracy
        delay(1);  // Introduces a small delay
}

/**
 * @brief Issues movement command at a specific velocity.
 *
 * @param dx X-axis movement delta
 * @param dy Y-axis movement delta
 * @param dz Z-axis movement delta
 * @return 0 on success, -1 on failure
 */
int moveat(int dx, int dy, int dz)
{
        int err = 0;

        // Retrieves an empty block for executing the movement
        block_t *block = block_buffer_get_empty();
        if (block == 0) {
                err = -1;  // No block available
        } else if (dx == 0 && dy == 0 && dz == 0) {
                reset();  // Stops movement if all deltas are zero
        } else {
                block->type = BLOCK_MOVEAT;  // Assigns moveat command type
                block->data[DT] = 1000;  // Assigns a default time step
                block->data[DX] = dx;
                block->data[DY] = dy;
                block->data[DZ] = dz;

                block_buffer_ready();  // Marks block for execution
        }

        return err;
}

/**
 * @brief Issues a timed movement command.
 *
 * @param dt Time duration for the movement
 * @param dx X-axis movement delta
 * @param dy Y-axis movement delta
 * @param dz Z-axis movement delta
 * @return 0 on success, -1 on failure
 */
int move(int dt, int dx, int dy, int dz)
{
        int err = 0;
        block_t *block = block_buffer_get_empty();
        if (block == 0) {
                err = -1;  // No block available
        } else {
                block->type = BLOCK_MOVE;  // Assigns move command type
                block->data[DT] = dt;
                block->data[DX] = dx;
                block->data[DY] = dy;
                block->data[DZ] = dz;
                block_buffer_ready();  // Marks block for execution
        }
        return err;
}

/**
 * @brief Handles the "moveto" command to move the device to a specific position.
 *
 * This function checks if the controller state is either RUNNING or PAUSED before
 * proceeding. It validates the provided delta time (DT) argument and retrieves an
 * empty movement block from the buffer. If the block is available, it populates it
 * with the parameters for the movement operation (delta time and displacements along
 * the X, Y, and Z axes) and marks the block as ready for processing. In case of
 * errors (e.g., missing block or invalid DT), specific error messages are sent back.
 *
 * @param romiSerial Pointer to the communication interface object.
 * @param args An array of arguments where:
 *             args[0] - Delta time for the movement.
 *             args[1] - X-axis displacement.
 *             args[2] - Y-axis displacement.
 *             args[3] - Z-axis displacement.
 * @param string_arg Additional string argument (unused in this handler).
 */
void handle_moveto(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Only proceed if the system is in RUNNING or PAUSED state
        if (controller_state == STATE_RUNNING
            || controller_state == STATE_PAUSED) {

                // Check if the 'DT' parameter (args[0]) is valid (greater than 0)
                if (args[0] > 0) {
                        // Retrieve an empty block from the block buffer for processing
                        block_t *block = block_buffer_get_empty();

                        // If no empty block is available, send an error message to request retry
                        if (block == 0) {
                                romiSerial->send_error(1, "Again");
                        } else {
                                // Populate the block with motion parameters
                                block->type = BLOCK_MOVETO;      // Set block type to 'MOVETO'
                                block->data[DT] = args[0];      // Set delta time (DT) for the move
                                block->data[DX] = args[1];      // Set X-axis displacement (DX)
                                block->data[DY] = args[2];      // Set Y-axis displacement (DY)
                                block->data[DZ] = args[3];      // Set Z-axis displacement (DZ)

                                // Mark the block as ready to be processed
                                block_buffer_ready();

                                // Notify the client that the operation succeeded
                                romiSerial->send_ok();
                        }
                } else {
                        // If 'DT' (args[0]) is invalid (<= 0), send an error message
                        romiSerial->send_error(100, "Invalid DT");
                }
        } else {
                // If the controller state is not valid for 'moveto', send an error
                romiSerial->send_error(101, kInvalidState);
        }
}

/**
 * @brief Handles a move command received via the serial interface.
 *
 * This function processes a move command by checking the current controller state
 * and validating the provided arguments. It invokes the `move` function to perform
 * the movement and sends an appropriate response back using the specified
 * `romiSerial` interface.
 *
 * @param romiSerial A pointer to the serial interface used for communication.
 * @param args An array of integers containing the move command arguments.
 *             args[0]: Command time duration (> 0 for valid command).
 *             args[1..3]: Parameters for the move operation.
 * @param string_arg A string argument passed to the function (not used).
 *
 * \note The function responds with:
 *       - `send_ok` if the move operation succeeds.
 *       - `send_error` if the arguments are invalid or the controller state is inappropriate.
 */
void handle_move(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_RUNNING
            || controller_state == STATE_PAUSED) {
                if (args[0] > 0) {
                        if (move(args[0], args[1], args[2], args[3]) == 0) {
                                romiSerial->send_ok();  
                        } else {
                                romiSerial->send_error(1, "Again");  
                        }
                } else {
                        romiSerial->send_error(100, "Invalid DT");                  
                }
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

/**
 * @brief Handles the "moveat" command for the robot, sending appropriate responses based on the result.
 *
 * This function checks the current controller state and only processes the "moveat" command
 * if the state is either STATE_RUNNING or STATE_PAUSED. It attempts to execute the "moveat"
 * operation using the provided arguments. If successful, it sends an OK response using the
 * RomiSerial interface; otherwise, it sends an error response based on specific failure conditions.
 * If the controller state is invalid for this operation, an error is sent indicating an invalid state.
 *
 * @param romiSerial A pointer to the RomiSerial interface for communication.
 * @param args       A pointer to an array of three int16_t arguments for the "moveat" command.
 * @param string_arg A C-style string argument (not used in this function).
 */
void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
    // Check if the current controller state allows executing "moveat".
    if (controller_state == STATE_RUNNING || controller_state == STATE_PAUSED) {

        // Attempt to execute the "moveat" operation with the provided arguments.
        if (moveat(args[0], args[1], args[2]) == 0) {
            // If "moveat" succeeds, send an OK response.
            romiSerial->send_ok();
        } else {
            // If "moveat" fails, send an error response with code 1 and message "Again".
            romiSerial->send_error(1, "Again");
        }

    } else {
        // If the controller state is invalid, send an error response with code 101
        // and the message for an invalid state.
        romiSerial->send_error(101, kInvalidState);
    }
}

/**
 * @brief Handles the pause command by transitioning the controller state.
 *
 * This function handles the `pause` command. If the controller is in the `STATE_RUNNING`
 * state, it disables the stepper timer and changes the state to `STATE_PAUSED`. If the controller
 * is already in `STATE_PAUSED`, it will simply acknowledge the command. For any other state,
 * it returns an error indicating an invalid state.
 *
 * @param romiSerial Pointer to the IRomiSerial interface for communication.
 * @param args An array of 16-bit integers that may hold additional arguments (unused here).
 * @param string_arg A string argument that may hold additional data (unused here).
 */
void handle_pause(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Check if the controller is in the running state
        if (controller_state == STATE_RUNNING) {
                // Disable the stepper timer to stop the motion
                disable_stepper_timer();
                // Update the controller state to paused
                controller_state = STATE_PAUSED;
                // Send a response acknowledging the successful pause command
                romiSerial->send_ok();
        }
        // Check if the controller is already in a paused state
        else if (controller_state == STATE_PAUSED) {
                // Acknowledge that the system is already paused
                romiSerial->send_ok();
        }
        // Handle invalid states (e.g., states other than running or paused)
        else {
                // Send an error response indicating an invalid state
                romiSerial->send_error(101, kInvalidState);  
        }
}

/**
 * @brief Handles the "continue" command, transitioning the controller state
 *        from PAUSED to RUNNING if appropriate.
 *
 * @param romiSerial Pointer to an object implementing the IRomiSerial interface.
 * @param args       An array of int16_t arguments (unused in this function).
 * @param string_arg String argument (unused in this function).
 */
void handle_continue(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED) {
                // Change the state from PAUSED to RUNNING.
                controller_state = STATE_RUNNING;

                // Enable the stepper motor timer for movement.
                enable_stepper_timer();

                // Send an acknowledgment of success.
                romiSerial->send_ok();
        } else if (controller_state == STATE_RUNNING) {
                // If already running, simply confirm with "OK".
                romiSerial->send_ok();
        } else {
                // Otherwise, send an error indicating invalid state.
                romiSerial->send_error(101, kInvalidState);
        }
}

/**
 * @brief Handles the "reset" command, resetting system parameters and transitioning
 *        state to RUNNING if the controller is in a valid state.
 *
 * @param romiSerial Pointer to an object implementing the IRomiSerial interface.
 * @param args       An array of int16_t arguments (unused in this function).
 * @param string_arg String argument (unused in this function).
 */
void handle_reset(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED) {
                // Transition state to RUNNING.
                controller_state = STATE_RUNNING;

                // Perform system reset and re-enable stepper timer.
                reset();
                enable_stepper_timer();

                // Send an acknowledgment of success.
                romiSerial->send_ok();
        } else if (controller_state == STATE_RUNNING) {
                // Reset system parameters while running.
                reset();

                // Confirm the command with "OK".
                romiSerial->send_ok();
        } else {
                // Send error for an invalid state.
                romiSerial->send_error(101, kInvalidState);
        }
}

/**
 * @brief Handles the "zero" command, setting the stepper position to zero
 *        if the controller is PAUSED or in an IDLE state.
 *
 * @param romiSerial Pointer to an object implementing the IRomiSerial interface.
 * @param args       An array of int16_t arguments (unused in this function).
 * @param string_arg String argument (unused in this function).
 */
void handle_zero(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED || is_idle()) {
                // Zero the stepper position.
                stepper_zero();

                // Send acknowledgment for successfully zeroing.
                romiSerial->send_ok();
        } else {
                // Return an error if not in a valid state.
                romiSerial->send_error(101, kInvalidState);
        }
}/**
 * @brief Sends the current stepper motor position through the serial interface.
 *
 * This function retrieves the position of all stepper motors, formats the data
 * into a reply string, and sends it via the provided serial interface.
 *
 * @param romiSerial Pointer to an IRomiSerial object for communication.
 * @param args Unused parameter, provided for API compatibility.
 * @param string_arg Unused parameter, provided for API compatibility.
 */
void send_position(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        int32_t pos[3];  // Array to store stepper motor positions.
        get_stepper_position(pos);  // Fetch current stepper motor positions.

        // Format the position data into a reply string.
        snprintf(reply_string, sizeof(reply_string),
                 "[0,%ld,%ld,%ld]", pos[0], pos[1], pos[2]);

        // Send the formatted reply string through the serial interface.
        romiSerial->send(reply_string);
}

/**
 * @brief Checks if the controller is idle.
 *
 * Determines if the system is idle by checking if the controller state
 * is in `STATE_RUNNING` and all steppers are not moving.
 *
 * @return true if the system is idle, false otherwise.
 */
static inline bool is_idle()
{
        // Check if controller is running and all steppers are idle.
        return ((controller_state == STATE_RUNNING)
                && stepper_is_idle());
}

/**
 * @brief Waits until the system becomes idle.
 *
 * Loops until the `is_idle` function returns true. While waiting,
 * it periodically handles serial input and introduces a short delay.
 */
static inline void wait()
{
        while (!is_idle()) {
                romiSerial.handle_input();  // Process incoming serial input.
                delay(1);  // Short delay to avoid busy-waiting.
        }
}

/**
 * @brief Sends the "idle" state of the controller through the serial interface.
 *
 * This function checks if the system is idle and formats the result along
 * with the current controller state into a reply string, which is then
 * transmitted via the provided serial interface.
 *
 * @param romiSerial Pointer to an IRomiSerial object for communication.
 * @param args Unused parameter, provided for API compatibility.
 * @param string_arg Unused parameter, provided for API compatibility.
 */
void send_idle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Format idle status and controller state into the reply string.
        snprintf(reply_string, sizeof(reply_string), "[0,%d,\"%c\"]",
                 is_idle(), controller_state);

        // Send the formatted reply string through the serial interface.
        romiSerial->send(reply_string);
}

/**
 * @brief Updates the state of all limit switches and reports changes.
 *
 * Reads the current states of the limit switches for the X, Y, and Z axes.
 * If any changes are detected in the Z-axis limit switch, it sends a message
 * through the serial interface to notify about the change.
 */
void update_limit_switches()
{
        // Update the state of the X, Y, and Z limit switches.
        limit_switches[0] = digitalRead(PIN_LIMIT_SWITCH_X);
        limit_switches[1] = digitalRead(PIN_LIMIT_SWITCH_Y);
        limit_switches[2] = digitalRead(PIN_LIMIT_SWITCH_Z);

        // Check if the state of the Z limit switch has changed.
        if (old_z != limit_switches[2])
        {
            old_z = limit_switches[2];  // Update the stored state.

            // Format a message to notify about the Z-axis limit switch change.
            snprintf(reply_string, sizeof(reply_string), "limit switch z changed [%d]", old_z);

            // Send the notification via the serial interface.
            romiSerial.send(reply_string);
        }
}
/**
 * @brief Performs a homing movement along the specified axis.
 *
 * @param dt      The time step or delay for the movement.
 * @param delta   The distance to move along the specified axis.
 * @param axis    The axis along which the movement is to be performed (0 = X, 1 = Y, 2 = Z).
 * @return int    Returns 0 on success, or an error code if the movement fails.
 */
int homing_move(int dt, int delta, int axis)
{
        int r;
        if (axis == 0) // Movement along the X-axis
                r = move(dt, delta, 0, 0);
        else if (axis == 1) // Movement along the Y-axis
                r = move(dt, 0, delta, 0);
        else if (axis == 2) // Movement along the Z-axis
                r = move(dt, 0, 0, delta);
        return r; // Return the result of the movement
}

/**
 * @brief Initiates a velocity-based homing movement along the specified axis.
 *
 * @param v       The speed for the movement.
 * @param axis    The axis along which the velocity is applied (0 = X, 1 = Y, 2 = Z).
 * @return int    Returns 0 on success, or an error code if the movement fails.
 */
int homing_moveat(int v, int axis)
{
        int r;
        if (axis == 0) // Velocity movement along the X-axis
                r = moveat(v, 0, 0);
        else if (axis == 1) // Velocity movement along the Y-axis
                r = moveat(0, v, 0);
        else if (axis == 2) // Velocity movement along the Z-axis
                r = moveat(0, 0, v);
        return r; // Return the result of the velocity movement
}

/**
 * @brief Waits for a limit switch to reach the desired state during a homing operation.
 *
 * Moves the actuator on the specified axis at a given speed, monitoring the limit switch
 * and halting movement when the switch reaches the target state.
 *
 * @param speed   The speed at which to perform the homing movement.
 * @param axis    The axis being homed (0 = X, 1 = Y, 2 = Z).
 * @param state   The desired state of the limit switch ('1' for activated, '0' for deactivated).
 * @return int    Returns 0 on success, or an error code if the movement fails.
 */
int homing_wait_switch(int speed, int axis, int state)
{
        int err = 0; // Variable to track errors during the operation
        int dx, dy, dz; // Unused variables for potential movement offsets

        // Start the homing movement at the given speed along the specified axis
        err = homing_moveat(speed, axis);
        if (err != 0) // If movement fails, return the error code
                return err;

        while (1) { // Continuous monitoring loop
                update_limit_switches(); // Update the state of the limit switches

                // Check if the axis's limit switch has reached the desired state
                if (limit_switches[axis] == state) {
                        err = move(0, 0, 0, 0); // Stop all movements
                        Serial.println("Homing complete"); // Debug message indicating success
                        break;
                }

                romiSerial.handle_input(); // Handle any input received during operation
                delay(1); // Introduce a short delay to avoid overwhelming the system
        }
        return err; // Return the error code or 0 if successful
}

/**
 * @brief Moves the specified axis towards the homing switch until it is pressed.
 *
 * This function initiates the homing process by moving the axis in the
 * negative direction at the respective homing speed, waiting until the
 * target switch is pressed.
 *
 * @param axis The axis index (0, 1, or 2) for which the homing operation is performed.
 * @return int Returns the status of the homing operation as determined by `homing_wait_switch`.
 */
int homing_moveto_switch_pressed(int axis)
{
        // Move the axis towards the switch at negative homing speed until it is pressed.
        return homing_wait_switch(-homing_speeds[axis], axis, LOW);
}

/**
 * @brief Moves the specified axis away from the homing switch until it is released.
 *
 * This function moves the axis in the positive direction at the respective
 * homing speed, waiting until the target switch is released.
 *
 * @param axis The axis index (0, 1, or 2) for which the homing operation is performed.
 * @return int Returns the status of the homing operation as determined by `homing_wait_switch`.
 */
int homing_moveto_switch_released(int axis)
{
        // Move the axis away from the switch at positive homing speed until it is released.
        return homing_wait_switch(homing_speeds[axis], axis, HIGH);
}

/**
 * @brief Performs the homing procedure for a specific axis.
 *
 * This function handles the homing routine for a specific axis.
 * The success or failure of the operation is logged for debugging purposes.
 *
 * @param axis The axis index (0, 1, or 2) to home.
 * @return bool Returns true if the homing operation is successful, false otherwise.
 */
bool do_homing_axis(int axis)
{
        // Begin the homing process
        Serial.print("HOMING ");
        Serial.print("axis ");
        Serial.print(axis);
        Serial.print("...:xxxx\r\n");
        bool success = false;
        if (homing_moveto_switch_pressed(axis) == 0
            && homing_moveto_switch_released(axis) == 0
            && homing_move(100, homing_speeds[axis]/5, axis) == 0) {
                // Don't remove the RUNNING because wait() depends on
                // it!
                controller_state = STATE_RUNNING;
                wait();
                success = true;
        }
        return success; // Return the final status of the homing routine.
}

/**
 * @brief Performs the homing procedure for all axes.
 *
 * This function ensures that all axes defined in `homing_axes` undergo
 * the homing routine. If any axis fails, the process stops.
 *
 * @return bool Returns true if all axes are successfully homed, false otherwise.
 */
bool do_homing()
{
        bool success = true;

        // Iterate over all defined axes and perform the homing operation.
        for (int i = 0; i < 3; i++) {
                // Ensure the axis is within valid range before proceeding.
                if (homing_axes[i] >= 0 && homing_axes[i] < 3) {
                        success = do_homing_axis(homing_axes[i]); // Perform homing for each valid axis.
                        if (!success)
                                break; // Exit on the first failure.
                }
        }
        return success; // Return the final status of the overall homing operation.
}


/**
 * @brief Handles the homing process, resetting current actions, enabling the stepper timer,
 *        and transitioning to the homing state. Sends an immediate response to the client.
 *
 * @param romiSerial Pointer to the IRomiSerial interface used for communication.
 * @param args Arguments for the homing function (unused in this handler).
 * @param string_arg String argument for the homing function (unused in this handler).
 */
void handle_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Stop current actions and reset system state.
        reset();

        // Ensure the stepper timer is active for the homing operation.
        enable_stepper_timer();

        // Set controller state to indicate homing is in progress.
        controller_state = STATE_HOMING;

        // Send an OK response immediately to avoid client timeouts.
        romiSerial->send_ok();

        // Attempt to perform the homing procedure.
        if (do_homing()) {
                // On success, reset, zero the stepper position, and transition to running state.
                reset();
                stepper_zero();
                controller_state = STATE_RUNNING;
        } else {
                // On failure, set the controller state to error.
                controller_state = STATE_ERROR;
        }
}

/**
 * @brief Configures the axes to be used for the homing operation.
 *
 * @param romiSerial Pointer to the IRomiSerial interface used for communication.
 * @param args Array containing the axis numbers for homing.
 * @param string_arg String argument for the function (unused in this handler).
 */
void handle_set_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Iterate through the first three axes and set them for homing.
        for (int i = 0; i < 3; i++)
                homing_axes[i] = args[i];

        // Acknowledge the configuration by sending an OK response.
        romiSerial->send_ok();
}

/**
 * @brief Enables or disables the motor driver based on input arguments.
 *
 * @param romiSerial Pointer to the IRomiSerial interface used for communication.
 * @param args Arguments indicating enable (1) or disable (0).
 * @param string_arg String argument for the function (unused in this handler).
 */
void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Check whether the driver should be disabled or enabled.
        if (args[0] == 0) {
                // Disable the motor driver.
                disable_driver();
        } else {
                // Enable the motor driver.
                enable_driver();
        }
        // Confirm the operation by sending an OK response.
        romiSerial->send_ok();
}

/**
 * @brief Controls the spindle state based on input arguments (on or off).
 *
 * @param romiSerial Pointer to the IRomiSerial interface used for communication.
 * @param args Arguments indicating spindle off (0) or spindle on (1).
 * @param string_arg String argument for the function (unused in this handler).
 */
void handle_spindle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Check whether to turn the spindle off or on.
        if (args[0] == 0) {
                // Turn the spindle off by setting the output pin low.
                digitalWrite(PIN_SPINLDE, LOW);
        } else {
                // Turn the spindle on by setting the output pin high.
                digitalWrite(PIN_SPINLDE, HIGH);
        }
        // Confirm the operation by sending an OK response.
        romiSerial->send_ok();
}

/**
 * @brief Sends formatted information over the provided serial interface.
 *
 * This function sends a predefined JSON-like message containing metadata,
 * including a version number ("0.1") and a timestamp (__DATE__ and __TIME__),
 * to the given `IRomiSerial` interface.
 *
 * @param romiSerial Pointer to an IRomiSerial instance used for communication.
 * @param args       Pointer to an array of integer arguments (currently unused).
 * @param string_arg Pointer to a constant character string argument (currently unused).
 */
void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Send a JSON-like string through the IRomiSerial object.
        // Includes module name ("Oquam"), version ("0.1"), and a timestamp (compile-time __DATE__ and __TIME__).
        romiSerial->send("[0,\"Oquam\",\"0.1\",\"" __DATE__ " " __TIME__ "\"]");
}

static bool quit_testing;

/**
 * @brief Starts a testing loop that continually polls the limit switches and handles serial input.
 *
 * This method initiates a test loop that runs until the `quit_testing` flag is set to true.
 * Inside the loop, it updates the state of the limit switches, prints their status,
 * and processes incoming serial commands through `romiSerial.handle_input()`.
 * The loop can be terminated by calling the `stop_test()` function, which sets `quit_testing` to true.
 */
void start_test()
{
        quit_testing = false; // Reset the quit flag to false to enter the testing loop.
        while (!quit_testing) {  // Continue running until `quit_testing` is set to true.

                // Uncomment the following code for motor movement testing:
                // Provide motor movement commands and introduce a delay for observation.

                moveat(1000, 1000, 100);  // Move forward with specified speeds and duration.
                delay(500);               // Wait for 500ms.

                moveat(-1000, -1000, -100); // Move backward with specified speeds and duration.
                delay(500);                 // Wait for 500ms.

                update_limit_switches(); // Update the states of the limit switches.

                // Print the states of the limit switches to the serial monitor.
                Serial.print("#![");
                Serial.print(limit_switches[0]);
                Serial.print(',');
                Serial.print(limit_switches[1]);
                Serial.print(',');
                Serial.print(limit_switches[2]);
                Serial.print("]:xxxx\r\n");

                romiSerial.handle_input(); // Process any incoming commands via the serial interface.
        }
}

/**
 * @brief Stops the testing loop and halts motor movement.
 *
 * This method sets the `quit_testing` flag to true, which breaks the testing loop in
 * `start_test()`. It also stops motor movement by calling `moveat` with all zero parameters.
 */
void stop_test()
{
        quit_testing = true; // Set the flag to true to terminate the testing loop.
        moveat(0, 0, 0);     // Stop all motor movement immediately.
}

/**
 * @brief Handles test mode based on received arguments.
 *
 * This function acts as a command handler for testing. When the first argument (`args[0]`) is 0,
 * it stops the testing loop by calling `stop_test()`. Otherwise, it starts the testing loop
 * by invoking `start_test()`. A confirmation is sent back via the serial interface.
 *
 * @param romiSerial Pointer to the ROMI serial interface for sending/receiving data.
 * @param args Array of integers representing command arguments.
 * @param string_arg Additional string argument (not used in this function).
 */
void handle_test(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        romiSerial->send_ok(); // Acknowledge receipt of the command over the serial interface.
        if (args[0] == 0) {
                stop_test(); // Stop the test if the argument is 0.
        } else {
                start_test(); // Start the test for any other argument value.
        }
}
