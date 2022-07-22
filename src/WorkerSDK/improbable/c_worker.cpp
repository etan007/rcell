#include "WorkerSDK/improbable/c_worker.h"

#include <stdbool.h>

#include "dynamic_msg_mgr.h"
#include "automaton/core/io/io.h"
#include "core/data/protobuf/protobuf_msg.h"
#include "core/data/msg.h"
const std::string path_to_log_file = "./";
using namespace automaton::core::io;
using automaton::core::data::protobuf::protobuf_msg;
using automaton::core::data::msg;

struct pbs_object
{
	std::unique_ptr<msg> msg_;
	std::vector<std::unique_ptr<pbs_object>> subobj;
};

struct pbs_ComponentData
{
	pbs_object obj;
};
struct Worker_SnapshotOutputStream_R
{
	std::ofstream* o;
	Worker_SnapshotState state;
};

uint32_t SKYCELL_APIVersion(void)
{
	return 0;
}

/**
 * Returns the SKYCELL_API_VERSION string that was defined when the library was compiled.
 */
const char* SKYCELL_APIVersionStr(void)
{
	return nullptr;
}

/**
 * Set custom allocation functions and state for managing memory within the API.
 * The allocation function should allocate a block of memory of the size that is given by
 * the argument and return a pointer to the first byte.
 * The pointer must be suitably aligned to hold an object of any fundamental alignment and
 * will be released by a matching call to the deallocation function with the same size.
 * If either allocation or deallocation function throws, the application will terminate.
 * Both allocation and deallocation functions must be thread-safe.
 *
 * You must call Worker_SetAllocator once before any other API calls. Calling it
 * multiple times or after another API call has been made is undefined behaviour.
 */
 void Worker_SetAllocator(Worker_AllocateFunction* allocate,
										  Worker_DeallocateFunction* deallocate, void* state)
{
}

/**
 * Returns a new Worker_ConnectionParameters with default values set.
 */
 Worker_ConnectionParameters Worker_DefaultConnectionParameters(void)
{
	 Worker_ConnectionParameters cp = {};
	 return cp;
}

/**
 * Creates a client which can be used to connect to a SpatialOS deployment via a locator service.
 * This is the standard flow used to connect a local worker to a cloud deployment.
 *
 * The hostname would typically be "locator.improbable.io".
 *
 * The port number is used to connect to the locator service. This should be set to zero if
 * connecting to a locator service running on the cloud.
 */
 Worker_Locator* Worker_Locator_Create(const char* hostname, uint16_t port,
												 const Worker_LocatorParameters* params)
{
	return nullptr;
}
/** Frees resources for a Worker_Locator created with Worker_Locator_Create. */
void Worker_Locator_Destroy(Worker_Locator* locator)
{
}

/**
 * (Deprecated) Queries the current list of deployments for the project given in the
 * Worker_LocatorParameters.
 */
Worker_DeploymentListFuture*
Worker_Locator_GetDeploymentListAsync(const Worker_Locator* locator)
{
	return nullptr;
}
/**
 * (Deprecated) Connects to a specific deployment. The deployment name should be obtained by calling
 * Worker_Locator_GetDeploymentListAsync. The callback should return zero to cancel queuing,
 * or non-zero to continue queueing.
 *
 * Returns a Worker_ConnectionFuture that can be used to obtain a Worker_Connection
 * by using Worker_ConnectionFuture_Get. Caller is responsible for destroying it when no
 * longer needed by using Worker_ConnectionFuture_Destroy.
 */
Worker_ConnectionFuture*
Worker_Locator_ConnectAndQueueAsync(const Worker_Locator* locator, const char* deployment_name,
									const Worker_ConnectionParameters* params, void* data,
									Worker_QueueStatusCallback* callback)
{
	return nullptr;
}

/**
 * Connects to a specific deployment. The deployment is defined by the login token,
 * which is obtained from a game authentication server, along with a player identity token.
 *
 * Returns a Worker_ConnectionFuture that can be used to obtain a Worker_Connection
 * by using Worker_ConnectionFuture_Get. Caller is responsible for destroying it when no
 * longer needed by using Worker_ConnectionFuture_Destroy.
 */
Worker_ConnectionFuture*
Worker_Locator_ConnectAsync(const Worker_Locator* locator,
							const Worker_ConnectionParameters* params)
{
	return nullptr;
}

/**
 * Connect to a SpatialOS deployment via a receptionist. This is the flow used to connect a managed
 * worker running in the cloud alongside the deployment, and also to connect any local worker to a
 * (local or remote) deployment via a locally-running receptionist.
 *
 * The hostname and port would typically be provided by SpatialOS on the command-line, if this is a
 * managed worker on the cloud, or otherwise be predetermined (e.g. localhost:7777 for the default
 * receptionist of a locally-running deployment).
 *
 * Returns a Worker_ConnectionFuture that can be used to obtain a Worker_Connection
 * by using Worker_ConnectionFuture_Get. Caller is responsible for destroying it when no
 * longer needed by using Worker_ConnectionFuture_Destroy.
 */
Worker_ConnectionFuture* Worker_ConnectAsync(const char* hostname, uint16_t port,
														const char* worker_id,
														const Worker_ConnectionParameters* params)
{
	return nullptr;
}

/** Destroys a Worker_DeploymentListFuture. Blocks until the future has completed. */
void Worker_DeploymentListFuture_Destroy(Worker_DeploymentListFuture* future)
{
}
/**
 * Gets the result of a Worker_DeploymentListFuture, waiting for up to *timeout_millis to
 * become available (or forever if timeout_millis is NULL).
 *
 * It is an error to call this method again once it has succeeded (e.g. not timed out) once.
 */
void Worker_DeploymentListFuture_Get(Worker_DeploymentListFuture* future,
												const uint32_t* timeout_millis, void* data,
												Worker_DeploymentListCallback* callback)
{
}

/** Calls the Development Authentication Service to generate a PIT. */
Worker_PlayerIdentityTokenResponseFuture*
Worker_CreateDevelopmentPlayerIdentityTokenAsync(
	const char* hostname, uint16_t port, Worker_PlayerIdentityTokenRequest* params)
{
	return nullptr;
}
/**
 * Destroys a Worker_PlayerIdentityTokenResponseFuture. Blocks until the future has
 * completed.
 */
void Worker_PlayerIdentityTokenResponseFuture_Destroy(
	Worker_PlayerIdentityTokenResponseFuture* future)
{
}
/**
 * Gets the result of a Worker_PlayerIdentityTokenResponseFuture, waiting for up to
 * *timeout_millis to become available (or forever if timeout_millis is NULL).
 *
 * It is an error to call this method again once it has succeeded (e.g. not timed out) once.
 */
void Worker_PlayerIdentityTokenResponseFuture_Get(
	Worker_PlayerIdentityTokenResponseFuture* future, const uint32_t* timeout_millis,
	void* data, Worker_PlayerIdentityTokenResponseCallback* callback)
{
}

/** Calls the Development Login Service to generate a login token list. */
Worker_LoginTokensResponseFuture*
Worker_CreateDevelopmentLoginTokensAsync(const char* hostname, uint16_t port,
											   Worker_LoginTokensRequest* params)
{
	return nullptr;
}
/**
 * Destroys a Worker_LoginTokensResponseFuture. Blocks until the future has
 * completed.
 */
void Worker_LoginTokensResponseFuture_Destroy(Worker_LoginTokensResponseFuture* future)
{
}
/**
 * Gets the result of a Worker_LoginTokensResponseFuture, waiting for up to
 * *timeout_millis to become available (or forever if timeout_millis is NULL).
 *
 * It is an error to call this method again once it has succeeded (e.g. not timeout out) once.
 */
void Worker_LoginTokensResponseFuture_Get(Worker_LoginTokensResponseFuture* future,
										   const uint32_t* timeout_millis, void* data,
										   Worker_LoginTokensResponseCallback* callback)
{
}

/** Destroys a Worker_ConnectionFuture. Blocks until the future has completed. */
void Worker_ConnectionFuture_Destroy(Worker_ConnectionFuture* future)
{
}
/**
 * Gets the result of a Worker_ConnectionFuture, waiting for up to *timeout_millis to
 * become available (or forever if timeout_millis is NULL). It returns NULL in case of a timeout.
 *
 * It is an error to call this method again once it has succeeded (e.g. not timed out) once.
 */
Worker_Connection* Worker_ConnectionFuture_Get(Worker_ConnectionFuture* future,
														  const uint32_t* timeout_millis)
{
	return nullptr;
}

/**
 * Frees resources for a Worker_Connection created with Worker_ConnectAsync or
 * Worker_Locator_ConnectAsync.
 */
void Worker_Connection_Destroy(Worker_Connection* connection)
{
}
/**
 * Indicates to the network layer that all previous invocations of the Worker_Connection_Send*
 * methods should be flushed to the network as soon as possible. A common usage pattern is to call
 * this function after all state changes have been applied at the end of a frame.
 *
 * This method is asynchronous. In particular, calling is an indication that a flush is wanted, and
 * can return before messages are fully serialized and put on the network.
 * This currently only has an effect if using the Worker_ModularKcpNetworkParameters or
 * Worker_ModularTcpNetworkParameters.
 */
void Worker_Connection_Flush(Worker_Connection* connection)
{
}
/** Sends a log message from the worker to SpatialOS. */
void Worker_Connection_SendLogMessage(Worker_Connection* connection,
												 const Worker_LogMessage* log_message)
{
}
/** Sends metrics data for the worker to SpatialOS. */
void Worker_Connection_SendMetrics(Worker_Connection* connection,
											  const Worker_Metrics* metrics)
{
}
/**
 * Requests SpatialOS to reserve multiple entity IDs.
 *
 * Note: We do not recommend reserving entity IDs. Instead, we recommend using
 * Worker_SendCreateEntityRequest without specifying an entity ID. You can then use the
 * automatically assigned entity ID provided in Worker_CreateEntityResponseOp.
 */
Worker_RequestId Worker_Connection_SendReserveEntityIdsRequest(
	Worker_Connection* connection, const uint32_t number_of_entity_ids,
	const uint32_t* timeout_millis)
{
	Worker_RequestId rid = 0; 
	return rid;
}
/**
 * Requests SpatialOS to create an entity. If components[i].schema_type is set, ownership is
 * transferred to the SDK, and components[i].schema_type is set to NULL. If
 * components[i].schema_type is NULL and components[i].user_handle is set, the entity data is
 * serialized immediately using the corresponding vtable serialize function.
 *
 * Returns Worker_RequestId -1 if the component is not registered and the default vtable is
 * not found.
 */
Worker_RequestId Worker_Connection_SendCreateEntityRequest(
	Worker_Connection* connection, uint32_t component_count, Worker_ComponentData* components,
	const Worker_EntityId* entity_id, const uint32_t* timeout_millis)
{
	Worker_RequestId rid = 0;
	return rid;
}
/** Requests SpatialOS to delete an entity. */
Worker_RequestId Worker_Connection_SendDeleteEntityRequest(
	Worker_Connection* connection, Worker_EntityId entity_id, const uint32_t* timeout_millis)
{
	Worker_RequestId rid = 0;
	return rid;
}
/**
 * Queries SpatialOS for entity data.
 *
 * Returns Worker_RequestId -1 if the query constraint or result type are not valid.
 */
Worker_RequestId Worker_Connection_SendEntityQueryRequest(
	Worker_Connection* connection, const Worker_EntityQuery* entity_query,
	const uint32_t* timeout_millis)
{
	Worker_RequestId rid = 0;
	return rid;
}
/**
 * Sends a component update for the given entity to SpatialOS.
 *
 * If component_update->schema_type is set, ownership is transferred to the SDK, and
 * component_update->schema_type is set to NULL. If component_update->user_handle is set, then it
 * will be copied with the corresponding vtable copy function, then the copy is later freed with the
 * vtable free function.
 *
 * Note that if update_parameters.loopback = 1 or update_parameters = NULL, the component update
 * operation is added to the operation list and will be returned by a subsequent call to
 * Worker_Connection_GetOpList.
 *
 * Returns WORKER_RESULT_FAILURE if the component is not registered and the default vtable is
 * not found.
 */
int8_t Worker_Connection_SendComponentUpdate(
	Worker_Connection* connection, Worker_EntityId entity_id,
	Worker_ComponentUpdate* component_update, const Worker_UpdateParameters* update_parameters)
{
	return 0;
}
/**
 * Adds a new component to the given entity in SpatialOS.
 *
 * If component_data->schema_type is set, ownership is transferred to the SDK, and
 * component_data->schema_type is set to NULL. If component_data->user_handle is set, then it
 * will be copied with the corresponding vtable copy function, then the copy is later freed with the
 * vtable free function.
 *
 * Note that if update_parameters.loopback = 1 or update_parameters = NULL, the add component
 * operation is added to the operation list and will be returned by a subsequent call to
 * Worker_Connection_GetOpList.
 *
 * Returns WORKER_RESULT_FAILURE if the component is not registered and the default vtable is
 * not found.
 */
int8_t Worker_Connection_SendAddComponent(
	Worker_Connection* connection, Worker_EntityId entity_id, Worker_ComponentData* component_data,
	const Worker_UpdateParameters* update_parameters)
{
	return 0;
}
/**
 * Removes a component from a given entity in SpatialOS.
 *
 * If update_parameters.loopback = 1 or update_parameters = NULL, the remove component operation
 * is added to the operation list and will be returned by a subsequent call to
 * Worker_Connection_GetOpList.
 *
 * In order to use this function, Worker_ConnectionParameters::enable_dynamic_components must be set
 * to 1 (true).
 *
 * This function does not check whether the worker currently has authority over the component, you
 * must make sure the worker has authority in order to remove the component.
 */
void Worker_Connection_SendRemoveComponent(Worker_Connection* connection, Worker_EntityId entity_id,
									  Worker_ComponentId component_id,
									  const Worker_UpdateParameters* update_parameters)
{
}
/**
 * Sends a command request targeting the given entity and component to SpatialOS. If timeout_millis
 * is null, the default will be used.
 *
 * If request->schema_type is set, ownership is transferred to the SDK, and request->schema_type is
 * set to NULL. If request->user_handle is set, then it will be copied with the corresponding vtable
 * copy function, then the copy is later freed with the vtable free function.
 *
 * If command parameters argument is NULL, then command short circuiting will be disabled.
 *
 * Returns Worker_RequestId -1 if the component is not registered and the default vtable is
 * not found.
 */
Worker_RequestId Worker_Connection_SendCommandRequest(
	Worker_Connection* connection, Worker_EntityId entity_id, Worker_CommandRequest* request,
	const uint32_t* timeout_millis, const Worker_CommandParameters* command_parameters)
{
	Worker_RequestId rid = 0;
	return rid;
}
/**
 * Sends a command response for the given request ID to SpatialOS.
 *
 * If response->schema_type is set, ownership is transferred to the SDK, and response->schema_type
 * is set to NULL. If response->user_handle is set, then it will be copied with the corresponding
 * vtable copy function, then the copy is later freed with the vtable free function.
 *
 * Returns WORKER_RESULT_FAILURE if the component is not registered and the default vtable is
 * not found.
 */
int8_t Worker_Connection_SendCommandResponse(Worker_Connection* connection,
														Worker_RequestId request_id,
														Worker_CommandResponse* response)
{
	return 0;
}
/** Sends an explicit failure for the given command request ID to SpatialOS. */
void Worker_Connection_SendCommandFailure(Worker_Connection* connection,
													 Worker_RequestId request_id,
													 const char* message)
{
}
/**
 * (Deprecated) Sends a diff-based component interest update for the given entity to SpatialOS. By
 * default, the worker receives data for all entities according to the default component interest
 * specified in its bridge settings. This function allows interest override by (entity ID, component
 * ID) pair to force the data to either always be sent or never be sent. Note that this does not
 * apply if the worker is _authoritative_ over a particular (entity ID, component ID) pair, in which
 * case the data is always sent.
 */
void Worker_Connection_SendComponentInterest(Worker_Connection* connection, Worker_EntityId entity_id,
										const Worker_InterestOverride* interest_override,
										uint32_t interest_override_count)
{
}
/**
 * Sends an acknowledgement of the receipt of an AuthorityLossImminent authority change for a
 * component. Sending the acknowledgement signifies that this worker is ready to lose authority
 * over the component.
 */
 void Worker_Connection_SendAuthorityLossImminentAcknowledgement(
	Worker_Connection* connection, Worker_EntityId entity_id, Worker_ComponentId component_id)
{
}
/**
 * (Deprecated) Enables or disables legacy protocol logging. Logging uses the parameters specified
 * when the connection was created. Enabling it when already enabled, or disabling it when already
 * disabled, does nothing.
 *
 * Note that logs from any previous protocol logging sessions will be overwritten.
 *
 * Either use this or the array of logsinks in Worker_ConnectionParameters. If you call this to
 * disable protocol logging while other logsinks are enabled, those will be disabled as well.
 */
void Worker_Connection_SetProtocolLoggingEnabled(Worker_Connection* connection,
															uint8_t enabled)
{
}
/**
 * Reenables all logging. If logging was already enabled or no logsinks had been configured during
 * connecting, does nothing. This also re-enables the deprecated protocol logging, if it was enabled
 * at least once using Worker_Connection_SetProtocolLoggingEnabled before.
 */
void Worker_Connection_EnableLogging(Worker_Connection* connection)
{
}
/**
 * Disables all logging. If logging was already disabled or no logsinks had been configured during
 * connecting, does nothing. This also disables the deprecated protocol logging, if it was enabled
 * at least once using Worker_Connection_SetProtocolLoggingEnabled before.
 */
void Worker_Connection_DisableLogging(Worker_Connection* connection)
{
}
/**
 * Returns true if the connection has been successfully created and communication is ongoing.
 * DEPRECATED: Equivalent to Worker_Connection_GetConnectionStatusCode(connection) ==
 *             WORKER_CONNECTION_STATUS_CODE_SUCCESS.
 */
uint8_t Worker_Connection_IsConnected(const Worker_Connection* connection)
{
	return 0;
}
/**
 * Returns a value from the Worker_ConnectionStatusCode enum. Returns
 * WORKER_CONNECTION_STATUS_SUCCESS if the connection is connected and usable, otherwise a
 * value indicating the type of error that occurred.
 */
uint8_t Worker_Connection_GetConnectionStatusCode(const Worker_Connection* connection)
{
	return 0;
}
/**
 * Returns a null terminated string containing more detailed information about the connection
 * status. The returned pointer points to data that is owned by the SDK and will remain valid for
 * the lifetime of the connection.
 */
const char* Worker_Connection_GetConnectionStatusDetailString(const Worker_Connection* connection)
{
	return nullptr;
}
/**
 * Retrieves the ID of the worker as assigned by the runtime as a null terminated string. The
 * returned pointer points to data that is owned by the SDK and will remain valid for the lifetime
 * of the connection. If the connection has failed, then the returned string will be a valid but
 * empty string.
 */
const char* Worker_Connection_GetWorkerId(const Worker_Connection* connection)
{
	return  nullptr;
}
/**
 * Retrieves the attributes associated with the worker at runtime. The result to data that is owned
 * by the SDK and will remain valid for the lifetime of the connection. If the connection has
 * failed, then the returned array of strings will be NULL.
 */
const Worker_WorkerAttributes* Worker_Connection_GetWorkerAttributes(const Worker_Connection* connection)
{
	return nullptr;
}
/**
 * Queries the worker flag with the given name. If the worker flag does not exist, the value will
 * be NULL.
 *
 * Worker flags are remotely configurable and may change during the runtime of the worker,
 * including addition and deletion.
 */
 void Worker_Connection_GetWorkerFlag(const Worker_Connection* connection,
												const char* name, void* user_data,
												Worker_GetWorkerFlagCallback* callback)
{
}

/**
 * Retrieves the list of operations that have occurred since the last call to this function.
 *
 * If timeout_millis is non-zero, the function will block until there is at least one operation to
 * return, or the timeout has been exceeded. If the timeout is exceeded, an empty list will be
 * returned.
 *
 * If timeout_millis is zero the function is non-blocking.
 *
 * It is the caller's responsibility to destroy the returned Worker_OpList with the
 * Worker_OpList_Destroy function.
 *
 * Note: All data contained within the op-list (such as component data or updates) is owned by
 * Worker_OpList, and must not be passed directly to another function in the SDK, such as
 * Worker_Connection_SendComponentUpdate, without copying the data first. Otherwise, a double free
 * could occur.
 */
Worker_OpList* Worker_Connection_GetOpList(Worker_Connection* connection,
													  uint32_t timeout_millis)
{
	return nullptr;
}
/** Frees resources for Worker_OpList returned by Worker_Connection_GetOpList. */
void Worker_OpList_Destroy(Worker_OpList* op_list)
{
}
 

/**
 * Opens a Worker_SnapshotInputStream. The caller must manage the memory of the
 * returned Worker_SnapshotInputStream* by calling Worker_SnapshotInputStream_Destroy to
 * write the EOF and release resources.
 *
 * If an error occurs, a pointer to a Worker_SnapshotInputStream is still returned.
 * Calling Worker_SnapshotInputStream_GetState with this pointer will return
 * an error message describing any error that occured. In the event of an error, the caller still
 * must release the memory of the Worker_SnapshotInputStream by calling
 * Worker_SnapshotInputStream_Destroy.
 */
Worker_SnapshotInputStream* Worker_SnapshotInputStream_Create(const char* filename, const Worker_SnapshotParameters* params)
{
	return nullptr;
}

/** Closes the SnapshotInputStream and releases its resources. */
void Worker_SnapshotInputStream_Destroy(Worker_SnapshotInputStream* input_stream)
{
}

/**
 * Returns zero (false) if the Worker_SnapshotInputStream has reached the EOF
 * of the Snapshot.
 */
uint8_t Worker_SnapshotInputStream_HasNext(Worker_SnapshotInputStream* input_stream)
{
	return 0;
}

/**
 * Reads next Worker_Entity* entity from input_stream.
 *
 * Worker_SnapshotInputStream_ReadEntity manages the memory for the returned entity internally. The
 * next call to Worker_SnapshotInputStream_ReadEntity or Worker_SnapshotInputStream_Destroy
 * invalidates this value; use Worker_AcquireComponentData as usual to preserve component data.
 *
 * If an error occurs, or the stream has reached the end of the file, then this function will return
 * a null pointer.
 *
 * In the case that a null pointer is returned, you must call Worker_SnapshotInputStream_GetState
 * to get the details of what error occurred during the operation.
 */
const Worker_Entity* Worker_SnapshotInputStream_ReadEntity(Worker_SnapshotInputStream* input_stream)
{
	return nullptr;
}

/**
 * Must be called after every Worker_SnapshotInputStream operation to get the state code of the
 * stream after the previous operation.
 *
 * Returns a Worker_SnapshotState which contains the stream state code and an error message: if the
 * code is WORKER_STREAM_STATE_GOOD no error occurred.
 */
Worker_SnapshotState Worker_SnapshotInputStream_GetState(Worker_SnapshotInputStream* input_stream)
{
	Worker_SnapshotState wss = {};
	return wss;
}

/**
 * Opens Worker_SnapshotOutputStream stream. The caller must manage the memory of the
 * returned Worker_SnapshotOutputStream* by calling
 * Worker_SnapshotOutputStream_Destroy to write the EOF and release resources.
 *
 * If an error occurs, a pointer to a Worker_SnapshotOutputStream is still returned.
 * Calling Worker_SnapshotOutputStream_GetState with this pointer will return
 * an error message describing any error that occured. In the event of an error, the caller still
 * must release the memory of the Worker_SnapshotOutputStream by calling
 * Worker_SnapshotOutputStream_Destroy.
 */
Worker_SnapshotOutputStream* Worker_SnapshotOutputStream_Create(const char* filename, const Worker_SnapshotParameters* params)
{
	Worker_SnapshotOutputStream_R* p = new Worker_SnapshotOutputStream_R();
	p->o = new std::ofstream(filename);
	p->state.stream_state = WORKER_STREAM_STATE_GOOD;
	p->state.error_message = nullptr;
	return (Worker_SnapshotOutputStream*)p;
}

/** Closes the snapshot output stream and releases its resources. */
void Worker_SnapshotOutputStream_Destroy(Worker_SnapshotOutputStream* output_stream)
{
	Worker_SnapshotOutputStream_R* p = (Worker_SnapshotOutputStream_R*)output_stream;
	
	 p->o->close();
	 delete p->o;
	 delete p;
}

/**
 * Writes next entity_id, entity pair from input. Must call Worker_SnapshotOutputStream_GetState
 * after this function to check whether any error occurred during operation.
 */
void Worker_SnapshotOutputStream_WriteEntity(Worker_SnapshotOutputStream* output_stream,
														const Worker_Entity* entity)
{
	Worker_SnapshotOutputStream_R* p = (Worker_SnapshotOutputStream_R*)output_stream;
	
	std::ofstream& o = *p->o;
	
	o << entity->entity_id << entity->component_count;
	for(uint32_t i=0;i<entity->component_count;i++)
	{
		auto data = entity->components[i];
		auto pbdata =  (pbs_ComponentData*)data.schema_type;
		std::string msgstr;
		if (pbdata->obj.msg_->serialize_message(&msgstr) )
		{
			o << data.component_id << msgstr.length();
			o.write( msgstr.data(),msgstr.length());
		}
		else
		{
			//o << data.component_id << msgstr;
		}
		 
	}
	
}

/**
 * Must be called after every Worker_SnapshotOutputStream operation to get
 * the state code of the stream after the previous operation.
 *
 * Returns a Worker_SnapshotState which contains the stream state code and an error
 * message: if the code is WORKER_STREAM_STATE_GOOD no error occurred.
 */
Worker_SnapshotState Worker_SnapshotOutputStream_GetState(Worker_SnapshotOutputStream* output_stream)
{
	Worker_SnapshotState wss = {};
	return wss;
}

/**
 * Returns the last warning message generated by a call to Worker_SnapshotOutputStream_WriteEntity.
 * Will return NULL if no warning was generated.
 */
const char* Worker_SnapshotOutputStream_GetLastWarning(Worker_SnapshotOutputStream* output_stream)
{
	return nullptr;
}

 Worker_EntityId Worker_Connection_GetWorkerEntityId(Worker_Connection* Connection)
{
	Worker_EntityId id = -1;
	return id;
}
 
 bool InitWorkLog()
{
	static bool bInitLog = false;
	if(!bInitLog)
	{
		auto worker = g3::LogWorker::createLogWorker();
		auto handle = worker->addDefaultLogger("worker", path_to_log_file);
		g3::initializeLogging(worker.get());
		bInitLog = true;
	}
	
	return true;
}
bool LoadAllSchema(const std::string& path,const std::string& json_path)
{
	InitWorkLog();
	return dynamic_msg_mgr::getMe().load(path,json_path);
}