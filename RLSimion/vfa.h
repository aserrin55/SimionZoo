#pragma once

class CStateFeatureMap;
class CActionFeatureMap;
class CFeatureList;
class CNamedVarSet;
typedef CNamedVarSet CState;
typedef CNamedVarSet CAction;
class CConfigNode;
class CConfigFile;

#include "parameters.h"
#include "deferred-load.h"
class IMemBuffer;

//CLinearVFA////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class CLinearVFA
{
protected:
	CFeatureList* m_pPendingUpdates= nullptr;
	IMemBuffer* m_pFrozenWeights = nullptr;
	IMemBuffer* m_pWeights= nullptr;
	size_t m_numWeights= 0;

	bool m_bSaturateOutput;
	double m_minOutput, m_maxOutput;

	bool m_bCanBeFrozen= false;

	size_t m_minIndex;
	size_t m_maxIndex;
public:
	CLinearVFA();
	virtual ~CLinearVFA();
	double get(const CFeatureList *features,bool bUseFrozenWeights= true);
	IMemBuffer *getWeights(){ return m_pWeights; }
	size_t getNumWeights(){ return m_numWeights; }

	void setCanUseDeferredUpdates(bool bCanUseDeferredUpdates);
	
	void add(const CFeatureList* pFeatures,double alpha= 1.0);

	void saturateOutput(double min, double max);

	void setIndexOffset(unsigned int offset);

	bool saveWeights(const char* pFilename) const;
	bool loadWeights(const char* pFilename);

};

class CLinearStateVFA: public CLinearVFA, public CDeferredLoad
{
protected:
	std::shared_ptr<CStateFeatureMap> m_pStateFeatureMap;
	CFeatureList *m_pAux;
	DOUBLE_PARAM m_initValue;
	virtual void deferredLoadStep();
public:
	CLinearStateVFA();
	CLinearStateVFA(CConfigNode* pParameters);

	void setInitValue(double initValue);

	virtual ~CLinearStateVFA();
	using CLinearVFA::get;
	double get(const CState *s);
	//double get(unsigned int featureIndex) const;

	void getFeatures(const CState* s,CFeatureList* outFeatures);
	void getFeatureState(unsigned int feature, CState* s);

	void save(const char* pFilename) const;

	std::shared_ptr<CStateFeatureMap> getStateFeatureMap(){ return m_pStateFeatureMap; }
};


class CLinearStateActionVFA : public CLinearVFA, public CDeferredLoad
{
protected:
	std::shared_ptr<CStateFeatureMap> m_pStateFeatureMap;
	std::shared_ptr<CActionFeatureMap> m_pActionFeatureMap;
	size_t m_numStateWeights;
	size_t m_numActionWeights;

	CFeatureList *m_pAux;
	CFeatureList *m_pAux2;
	DOUBLE_PARAM m_initValue;
	int *m_pArgMaxTies= nullptr;

public:
	size_t getNumStateWeights() const{ return m_numStateWeights; }
	size_t getNumActionWeights() const { return m_numActionWeights; }
	std::shared_ptr<CStateFeatureMap> getStateFeatureMap() { return m_pStateFeatureMap; }
	std::shared_ptr<CActionFeatureMap> getActionFeatureMap() { return m_pActionFeatureMap; }

	CLinearStateActionVFA()= default;
	CLinearStateActionVFA(CConfigNode* pParameters);
	CLinearStateActionVFA(CLinearStateActionVFA* pSourceVFA); //used in double q-learning to getSample a copy of the target function
	CLinearStateActionVFA(std::shared_ptr<CStateFeatureMap> pStateFeatureMap
		, std::shared_ptr<CActionFeatureMap> pActionFeatureMap);

	void setInitValue(double initValue);

	virtual ~CLinearStateActionVFA();
	using CLinearVFA::get;
	double get(const CState *s, const CAction *a);
	//double get(unsigned int sFeatureIndex,unsigned int aFeatureIndex) const;

	void argMax(const CState *s, CAction* a);
	double max(const CState *s, bool bUseFrozenWeights= true);
	
	//This function fills the pre-allocated array outActionVariables with the values of the different actions in state s
	//The size of the buffer must be greater than the number of action weights
	void getActionValues(const CState* s, double *outActionValues);

	void getFeatures(const CState* s, const CAction* a, CFeatureList* outFeatures);

	//features are built using the two feature maps: the state and action feature maps
	//the input is a feature in state-action space
	void getFeatureStateAction(unsigned int feature,CState* s, CAction* a);

	void deferredLoadStep();
};