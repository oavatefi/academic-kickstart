/**
 * \file
 *
 * \brief AUTOSAR E2EPW
 *
 * This file contains the implementation of the AUTOSAR
 * module E2EPW.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2013 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (!defined _E2EPW_INT_CFG_H_)
#define _E2EPW_INT_CFG_H_

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

#if (defined E2E_USING_RTE_ISUPDATED) /* To prevent double definition */
#error E2E_USING_RTE_ISUPDATED already defined
#endif /* (defined E2E_USING_RTE_ISUPDATED) */

/**
 * \brief Defines if Rte_IsUpdated (provided by RTE) shall be used by E2EPW
 * STD_ON  - Rte_IsUpdated shall be used
 * STD_OFF - Rte_IsUpdated shall not be used
 */
#define E2E_USING_RTE_ISUPDATED STD_ON

/* E2E protection description for 01 */

#if (defined E2EPW_ENABLE_PROFILE_P01) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_P01 already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_P01) */

/** \brief Defines if E2E Profile P01 is used */
#define E2EPW_ENABLE_PROFILE_P01 STD_OFF

/* E2E protection description for 02 */

#if (defined E2EPW_ENABLE_PROFILE_P02) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_P02 already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_P02) */

/** \brief Defines if E2E Profile P02 is used */
#define E2EPW_ENABLE_PROFILE_P02 STD_ON

/* E2E protection description for 03 */

#if (defined E2EPW_ENABLE_PROFILE_P03) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_P03 already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_P03) */

/** \brief Defines if E2E Profile P03 is used */
#define E2EPW_ENABLE_PROFILE_P03 STD_OFF

/* E2E protection description for DAI1 */

#if (defined E2EPW_ENABLE_PROFILE_PDAI1) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_PDAI1 already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_PDAI1) */

/** \brief Defines if E2E Profile PDAI1 is used */
#define E2EPW_ENABLE_PROFILE_PDAI1 STD_OFF

/* E2E protection description for DAI2 */

#if (defined E2EPW_ENABLE_PROFILE_PDAI2) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_PDAI2 already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_PDAI2) */

/** \brief Defines if E2E Profile PDAI2 is used */
#define E2EPW_ENABLE_PROFILE_PDAI2 STD_OFF

/* E2E protection description for VCC */

#if (defined E2EPW_ENABLE_PROFILE_PVCC) /* to prevent double declaration */
#error E2EPW_ENABLE_PROFILE_PVCC already defined
#endif /* (defined E2EPW_ENABLE_PROFILE_PVCC) */

/** \brief Defines if E2E Profile PVCC is used */
#define E2EPW_ENABLE_PROFILE_PVCC STD_OFF


/*==[Types]===================================================================*/

/*==[Declaration of Constants with external linkage]==========================*/

/*==[Declaration of Variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* if !defined(_E2EPW_INT_CFG_H_) */
/*==================[end of file]===========================================*/
