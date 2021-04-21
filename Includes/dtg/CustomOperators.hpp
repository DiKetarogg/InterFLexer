#pragma once
#ifndef DTG_CUSTOMOPERATORS_HPP
#define DTG_CUSTOMOPERATORS_HPP

namespace dtg {

	template<class T, class A>
	struct CustomOperator {
		CustomOperator() = delete;
		explicit CustomOperator(T data):data(data){}
		operator T() {
			return data;
		}
		T data;
	};

	enum { CUSTOM_OPERATOR_LESS };
	enum { CUSTOM_OPERATOR_LESSEQ };
	enum { CUSTOM_OPERATOR_EQ };
	enum { CUSTOM_OPERATOR_NEQ };
	enum { CUSTOM_OPERATOR_GREAT };
	enum { CUSTOM_OPERATOR_GREATEQ };
	enum { CUSTOM_OPERATOR_SET };
}

#define dtg_less_ <dtg::CUSTOM_OPERATOR_LESS>
#define dtg_lesseq_ <dtg::CUSTOM_OPERATOR_LESSEQ>
#define dtg_eq_ ==dtg::CUSTOM_OPERATOR_EQ==
#define dtg_neq_ !=dtg::CUSTOM_OPERATOR_NEQ!=
#define dtg_great  <dtg::CUSTOM_OPERATOR_GREAT>
#define dtg_greateq_ <dtg::CUSTOM_OPERATOR_GREATEQ>

#define dtg_set_ =dtg::CUSTOM_OPERATOR_SET=

#ifdef DTG_CUSTOM_OPERATORS_NO_PREFIX
#define less_ <dtg::CUSTOM_OPERATOR_LESS>
#define lesseq_ <dtg::CUSTOM_OPERATOR_LESSEQ>
#define eq_ ==dtg::CUSTOM_OPERATOR_EQ==
#define neq_ !=dtg::CUSTOM_OPERATOR_NEQ!=
#define great_ <dtg::CUSTOM_OPERATOR_GREAT>
#define greateq_ <dtg::CUSTOM_OPERATOR_GREATEQ>
#endif


#define DTG_BUILD_CUSTOM_BIN_LEFT(Type, operator1, enumname)\
inline dtg::CustomOperator<Type, decltype(enumname)> operator operator1(Type left, decltype(enumname)) {\
	return dtg::CustomOperator<Type, decltype(enumname)>(left);\
}

//Make sure to cast left hand operand to the needed type
#define DTG_BUILD_CUSTOM_BIN_RIGHT(attributes, Type, enumname, operator2)\
attributes auto operator operator2(const dtg::CustomOperator<Type, decltype(enumname)>& left, Type right)

//Make sure to cast left hand operand to the needed type
#define DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, operator1, enumname, operator2)\
DTG_BUILD_CUSTOM_BIN_LEFT(Type, operator1, enumname)\
DTG_BUILD_CUSTOM_BIN_RIGHT(attributes, Type, enumname, operator2)

//Make sure to cast left hand operand to the needed type
#define DTG_BUILD_CUSTOM_SPACESHIP(attributes, Type, leftexp, rightexp,\
enumless, enumlesseq, enumeq, enumneq, enumgreat, enumgreateq)\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, <, enumless, >)->bool {\
	return leftexp < rightexp;\
}\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, <, enumlesseq, >)->bool {\
	return leftexp <= rightexp;\
}\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, ==, enumeq, ==)->bool {\
	return leftexp == rightexp;\
}\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, !=, enumneq, !=)->bool {\
	return leftexp != rightexp;\
}\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, <, enumgreat, >)->bool {\
	return leftexp > rightexp;\
}\
DTG_BUILD_CUSTOM_BIN_BOTH(attributes, Type, <, enumgreateq, >)->bool {\
	return leftexp >= rightexp;\
}

#define DTG_BUILD_CUSTOM_DTG_SPACESHIP(attributes, Type, leftexp, rightexp)\
DTG_BUILD_CUSTOM_SPACESHIP(attributes, Type, leftexp, rightexp,\
dtg::CUSTOM_OPERATOR_LESS, dtg::CUSTOM_OPERATOR_LESSEQ, dtg::CUSTOM_OPERATOR_EQ,\
dtg::CUSTOM_OPERATOR_NEQ, dtg::CUSTOM_OPERATOR_GREAT, dtg::CUSTOM_OPERATOR_GREATEQ)


#endif
