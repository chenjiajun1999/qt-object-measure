#pragma once

#include "Common.h"

#define SHORT_AUTO_PROPERTY(TYPE, NAME) AUTO_PROPERTY(TYPE, NAME, get##NAME, set##NAME, NAME##Changed)

/**
  Internal helper to create member for usage as AUTO_PROPERTY
*/
#define ADD_AUTO_PROPERTY_MEMBERS(type, member_type, name, getter_visibility, getter, setter_visibility, setter, notify) \
    getter_visibility: \
        type getter() const \
        { \
            return name; \
        } \
    setter_visibility Q_SLOTS: \
        void setter(type value) \
        { \
            if (updateIfChanged(name, value)) \
            { \
                emit notify(value); \
            } \
        } \
    Q_SIGNALS: \
        void notify(type name); \
    private: \
        member_type name {};

/**
  Macro for generating simple properties automatically

  It creates member, getter, setter and notify signal.

  Should be used in private section.

  Q_SIGNALS macro is used instead of signals keyword, because it wasn't working with the latter
*/
#define AUTO_PROPERTY(type, name, getter, setter, notify) \
    Q_PROPERTY(type name READ getter WRITE setter NOTIFY notify) \
    ADD_AUTO_PROPERTY_MEMBERS(type, type, name, public, getter, public, setter, notify)

#define AUTO_MEMBER_PROPERTY(type, name) \
    Q_PROPERTY(type name MEMBER name) \
    public: \
        type name {};

/**
  Macro for generating simple read only properties automatically
*/
#define AUTO_READONLY_PROPERTY(type, name, getter, setter, notify) \
    Q_PROPERTY(type name READ getter NOTIFY notify) \
    ADD_AUTO_PROPERTY_MEMBERS(type, type, name, public, getter, public, setter, notify)

/**
  Macro for generating simple write only properties automatically
*/
#define AUTO_WRITEONLY_PROPERTY(type, name, setter, notify) \
    Q_PROPERTY(type name READ name WRITE setter NOTIFY notify) \
    ADD_AUTO_PROPERTY_MEMBERS(type, type, name, protected, name, public, setter, notify)
