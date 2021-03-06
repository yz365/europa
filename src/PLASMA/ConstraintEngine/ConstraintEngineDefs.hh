#ifndef _H_ConstraintEngineDefs
#define _H_ConstraintEngineDefs

/**
 * Just provide forward declarations of classes and certain global declarations related to error handling.
 */

#include "CommonDefs.hh"
#include "Id.hh"
#include "Entity.hh"

#include <list>
#include <set>

namespace EUROPA {
class DataType;
typedef Id<DataType> DataTypeId;

class Domain;
class IntervalDomain;
class IntervalIntDomain;
class BoolDomain;
class EnumeratedDomain;

class DomainListener;
typedef Id<DomainListener> DomainListenerId;

class ConstrainedVariable;
typedef Id<ConstrainedVariable> ConstrainedVariableId;
typedef std::set<ConstrainedVariableId, EntityComparator<ConstrainedVariableId> > ConstrainedVariableSet;

class ConstrainedVariableListener;
typedef Id<ConstrainedVariableListener> ConstrainedVariableListenerId;
class VariableChangeListener;
typedef Id<VariableChangeListener> VariableChangeListenerId;

class Constraint;
typedef Id<Constraint> ConstraintId;
typedef std::set<ConstraintId, EntityComparator<ConstraintId> > ConstraintSet;
class CFunction;
typedef Id<CFunction> CFunctionId;
class ConstraintType;
typedef Id<ConstraintType> ConstraintTypeId;
class Propagator;
typedef Id<Propagator> PropagatorId;
class ConstraintEngine;
typedef Id<ConstraintEngine> ConstraintEngineId;
class ConstraintEngineListener;
typedef Id<ConstraintEngineListener> ConstraintEngineListenerId;
typedef std::pair<ConstraintId, unsigned int> ConstraintEntry;
typedef std::list<ConstraintEntry> ConstraintList;

class CESchema;
typedef Id<CESchema> CESchemaId;

class PostPropagationCallback;
typedef Id<PostPropagationCallback> PostPropagationCallbackId;

class PSVarValue;
/**
 * @brief Helper method to cast singleton values
 */
template<class T>
Id<T> id(const ConstrainedVariableId var);

class StringDomain;
typedef StringDomain LabelSet;

/** Constants **/
DECLARE_GLOBAL_CONST(std::string, g_noVarName);
#define NO_VAR_NAME (g_noVarName())

} /* namespace EUROPA */

#endif /* #ifndef _H_ConstraintEngineDefs */
