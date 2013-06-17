/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */

/*
 * These routines manipulate a low level buffer assuming it holds
 * an OpenFlow message. 
 */

#if !defined(_OF_MESSAGE_H_)
#define _OF_MESSAGE_H_

#include <loci/of_buffer.h>

typedef uint8_t *of_message_t;

/* A few key common header offsets */
#define OF_MESSAGE_VERSION_OFFSET 0
#define OF_MESSAGE_TYPE_OFFSET 1
#define OF_MESSAGE_LENGTH_OFFSET 2
#define OF_MESSAGE_XID_OFFSET 4
#define OF_MESSAGE_HEADER_LENGTH 8
#define OF_MESSAGE_STATS_TYPE_OFFSET 8
#define OF_MESSAGE_FLOW_MOD_COMMAND_OFFSET(version) ((version) == 1 ? 56 : 25)

#define OF_MESSAGE_MIN_LENGTH 8
#define OF_MESSAGE_MIN_STATS_LENGTH (OF_MESSAGE_STATS_TYPE_OFFSET + 2)
#define OF_MESSAGE_MIN_FLOW_MOD_LENGTH(version)  ((version) == 1 ? 57 : 26)

#define OF_MESSAGE_EXPERIMENTER_ID_OFFSET 8
#define OF_MESSAGE_EXPERIMENTER_SUBTYPE_OFFSET 12
#define OF_MESSAGE_EXPERIMENTER_MIN_LENGTH 16

/**
 * The "default" free message function; NULL means use nominal malloc/free
 */
#define OF_MESSAGE_FREE_FUNCTION NULL

/**
 * Map a message to the uint8_t * start of the message
 */
#define OF_MESSAGE_TO_BUFFER(msg) ((uint8_t *)(msg))

/**
 * Map a uint8_t * to a message object
 */
#define OF_BUFFER_TO_MESSAGE(buf) ((of_message_t)(buf))

/****************************************************************
 *
 * Message field accessors.
 *
 * These do no range checking and assume a buffer with sufficient
 * length to access the data.  These are low level accessors used
 * during the parsing and coersion stage of message processing.
 *
 * Fields include:  version, message type, message length,
 * transaction id, stats type (now multi-part type), experimenter id,
 * experimenter type
 *
 ****************************************************************/

/**
 * @brief Get/set version of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param version Data for set operation
 * @returns get returns version
 */

static inline of_version_t
of_message_version_get(of_message_t msg) {
    return (of_version_t)msg[OF_MESSAGE_VERSION_OFFSET];
}

static inline void
of_message_version_set(of_message_t msg, of_version_t version) {
    buf_u8_set(msg, (uint8_t)version);
}

/**
 * @brief Get/set OpenFlow type of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param value Data for set operation
 * @returns get returns message type
 */

static inline uint8_t
of_message_type_get(of_message_t msg) {
    return msg[OF_MESSAGE_TYPE_OFFSET];
}

static inline void
of_message_type_set(of_message_t msg, uint8_t value) {
    buf_u8_set(msg + OF_MESSAGE_TYPE_OFFSET, value);
}

/**
 * @brief Get/set in-buffer length of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param len Data for set operation
 * @returns get returns length in host order
 */

static inline uint16_t
of_message_length_get(of_message_t msg) {
    uint16_t val;
    buf_u16_get(msg + OF_MESSAGE_LENGTH_OFFSET, &val);
    return val;
}

static inline void
of_message_length_set(of_message_t msg, uint16_t len) {
    buf_u16_set(msg + OF_MESSAGE_LENGTH_OFFSET, len);
}


/**
 * @brief Get/set transaction ID of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param xid Data for set operation
 * @returns get returns xid in host order
 */

static inline uint32_t
of_message_xid_get(of_message_t msg) {
    uint32_t val;
    buf_u32_get(msg + OF_MESSAGE_XID_OFFSET, &val);
    return val;
}

static inline void
of_message_xid_set(of_message_t msg, uint32_t xid) {
    buf_u32_set(msg + OF_MESSAGE_XID_OFFSET, xid);
}

/**
 * @brief Get/set stats type of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param type Data for set operation
 * @returns get returns stats type in host order
 */

static inline uint16_t
of_message_stats_type_get(of_message_t msg) {
    uint16_t val;
    buf_u16_get(msg + OF_MESSAGE_STATS_TYPE_OFFSET, &val);
    return val;
}

static inline void
of_message_stats_type_set(of_message_t msg, uint16_t type) {
    buf_u16_set(msg + OF_MESSAGE_STATS_TYPE_OFFSET, type);
}


/**
 * @brief Get/set experimenter ID of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param experimenter_id Data for set operation
 * @returns get returns experimenter id in host order
 */

static inline uint32_t
of_message_experimenter_id_get(of_message_t msg) {
    uint32_t val;
    buf_u32_get(msg + OF_MESSAGE_EXPERIMENTER_ID_OFFSET, &val);
    return val;
}

static inline void
of_message_experimenter_id_set(of_message_t msg, uint32_t experimenter_id) {
    buf_u32_set(msg + OF_MESSAGE_EXPERIMENTER_ID_OFFSET, experimenter_id);
}


/**
 * @brief Get/set experimenter message type (subtype) of a message
 * @param msg Pointer to the message buffer of sufficient length
 * @param subtype Data for set operation
 * @returns get returns experimenter message type in host order
 */

static inline uint32_t
of_message_experimenter_subtype_get(of_message_t msg) {
    uint32_t val;
    buf_u32_get(msg + OF_MESSAGE_EXPERIMENTER_SUBTYPE_OFFSET, &val);
    return val;
}

static inline void
of_message_experimenter_subtype_set(of_message_t msg,
                                    uint32_t subtype) {
    buf_u32_set(msg + OF_MESSAGE_EXPERIMENTER_SUBTYPE_OFFSET,
                subtype);
}

/**
 * Flow mod command changed from 16 to 8 bits on the wire from 1.0 to 1.1
 */
static inline uint8_t
of_message_flow_mod_command_get(of_message_t msg, of_version_t version) {
    uint16_t val16;
    uint8_t val8;

    if (version == OF_VERSION_1_0) {
        buf_u16_get(msg + OF_MESSAGE_FLOW_MOD_COMMAND_OFFSET(version), &val16);
        val8 = val16;
    } else {
        buf_u8_get(msg + OF_MESSAGE_FLOW_MOD_COMMAND_OFFSET(version), &val8);
    }
    return val8;
}

static inline void
of_message_flow_mod_command_set(of_message_t msg, of_version_t version, 
                                uint8_t command) {
    uint16_t val16;

    if (version == OF_VERSION_1_0) {
        val16 = command;
        buf_u16_set(msg + OF_MESSAGE_FLOW_MOD_COMMAND_OFFSET(version), val16);
    } else {
        buf_u8_set(msg + OF_MESSAGE_FLOW_MOD_COMMAND_OFFSET(version), command);
    }
}

#endif /* _OF_MESSAGE_H_ */
