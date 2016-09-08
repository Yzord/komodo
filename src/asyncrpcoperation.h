// Copyright (c) 2016 The Zcash developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef ASYNCRPCOPERATION_H
#define ASYNCRPCOPERATION_H

#include <string>
#include <atomic>
#include <map>
#include <chrono>

#include "json/json_spirit_value.h"
#include "json/json_spirit_utils.h"
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"

using namespace std;
using namespace json_spirit;

/**
 * AsyncRPCOperation objects are submitted to the AsyncRPCQueue for processing.
 * 
 * To subclass AsyncRPCOperation, implement the main() method.
 * Update the operation status as work is underway and completes.
 * If main() can be interrupted, inmplement the cancel() method.
 */

typedef std::string AsyncRPCOperationId;

typedef enum class operationStateEnum {
    READY = 0,
    EXECUTING,
    CANCELLED,
    FAILED,
    SUCCESS
} OperationStatus;

class AsyncRPCOperation {
public:
    AsyncRPCOperation();
    virtual ~AsyncRPCOperation();

    // You must implement this method in your subclass.
    virtual void main();

    // Override this method if you can interrupt execution of main() in your subclass.
    void cancel();
    
    // Getters and setters

    OperationStatus getState() const {
        return state_.load();
    }
        
    AsyncRPCOperationId getId() const {
        return id_;
    }
   
    int64_t getCreationTime() const {
        return creation_time_;
    }

    Value getStatus() const;

    Value getError() const;
    
    Value getResult() const;

    std::string getStateAsString() const;
    
    int getErrorCode() const {
        return error_code_;
    }

    std::string getErrorMessage() const {
        return error_message_;
    }

    bool isCancelled() const {
        return OperationStatus::CANCELLED == getState();
    }

    bool isExecuting() const {
        return OperationStatus::EXECUTING == getState();
    }

    bool isReady() const {
        return OperationStatus::READY == getState();
    }

    bool isFailed() const {
        return OperationStatus::FAILED == getState();
    }
    
    bool isSuccess() const {
        return OperationStatus::SUCCESS == getState();
    }

protected:
    // The state_ is atomic because only it can be mutated externally.
    // For example, the user initiates a shut down of the application, which closes
    // the AsyncRPCQueue, which in turn invokes cancel() on all operations.
    // The member variables below are protected rather than private in order to
    // allow subclasses of AsyncRPCOperation the ability to access and update
    // internal state.  Currently, all operations are executed in a single-thread
    // by a single worker.
    Value result_;
    int error_code_;
    std::string error_message_;
    std::atomic<OperationStatus> state_;
    std::chrono::time_point<std::chrono::system_clock> start_time_, end_time_;  

    void start_execution_clock();
    void stop_execution_clock();

    void set_state(OperationStatus state) {
        this->state_.store(state);
    }

    void set_error_code(int errorCode) {
        this->error_code_ = errorCode;
    }

    void set_error_message(std::string errorMessage) {
        this->error_message_ = errorMessage;
    }
    
    void set_result(Value v) {
        this->result_ = v;
    }
    
private:

    // Derived classes should write their own copy constructor and assignment operators
    AsyncRPCOperation(const AsyncRPCOperation& orig);
    AsyncRPCOperation& operator=( const AsyncRPCOperation& other );
    
    void set_id(AsyncRPCOperationId id) {
        this->id_ = id;
    }
    
    void set_creation_time(int64_t creationTime) {
        this->creation_time_ = creationTime;
    }

    AsyncRPCOperationId id_;
    int64_t creation_time_;
};

#endif /* ASYNCRPCOPERATION_H */
