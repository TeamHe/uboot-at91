/*
 * (C) Copyright 2000-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2001 Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Andreas Heppel <aheppel@sysgo.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <env_callback.h>


//#if defined(CONFIG_CCU_800)

#define CCU800_CONSOLE		"ttyS0,115200"
#define CCU800_SWITCH_OS1	"if test ${active} = undefined || test ${active} = master; then " \
								"setenv active slave ; " \
							"else " \
								"setenv active master; " \
							"fi;"

#define CCU800_SWITCH_OS	"setenv cfg_start *; setenv switch false; " \
							"run switch_os1; " 							\
							"saveenv; " 								\
							"echo switch os;"

#define CONFIG_CCU_CHECK_3		"if test ${cfg_start} = **; then " 		\
									"setenv cfg_start ***; saveenv; " 	\
								"else  run switch_os; " 				\
								"fi; " 									\
								"echo check-3;" 						
#define CONFIG_CCU_CHECK_2		"if test ${cfg_start} = *; then " 		\
									"setenv cfg_start **; saveenv; " 	\
								"else run check_3; " 					\
								"fi; " 									\
								"echo check-2;" 
#define	CONFIG_CCU_CHECK_1		"if test ! ${switch} = undefined;  then " 		\
									"if test ${switch} = true; then " 			\
										"run switch_os;  " 						\
									"else run check_2; " 						\
									"fi; " 										\
								"fi; " 											\
								"echo check-1;"
#define CONFIG_CCU_OS1_INIT		"setenv mtdparts atmel_nand:1M(boot)ro," 		\
										"512K(env),8M(kernel)ro,72M(rootfs)," 	\
										"8M(os2),72M(os2root),-(data); " 		\
								"setenv ubi.tmd 3; " 							\
								"setenv dtb_os 0x180000; " 						\
								"setenv kernel_os 0x200000;" 	
#define CONFIG_CCU_OS2_MTDPARTS "atmel_nand:1M(boot)ro,512K(env)," 				\
											"8M(os1),72M(os1root)," 			\
											"8M(kernel)ro,72M(rootfs),-(data)"
#define CONFIG_CCU_OS2_DTB		"0x5180000"
#define CONFIG_CCU_OS2_KERNEL	"0x5200000"
#define CONFIG_CCU_OS2_ROOTFS	"0x5980000"
#define	CONFIG_CCU_OS2_UBITMD	"4"
#define CONFIG_CCU_OS2_INIT		"setenv mtdparts ${os2_mtdparts}; " 			\
								"setenv ubi.tmd  ${os2_ubi.tmd};" 				\
								"setenv dtb_os ${os2_dtb}; " 					\
								"setenv kernel_os ${os2_kernel};"
#define CONFIG_CCU_INIT_OS		"if test ${active} = undefined || " 			\
										 "test ${active} = master; then "  		\
									"run os1_init; echo os1_start; " 			\
								"else " 										\
									"run os2_init; echo os2_start; " 			\
								"fi;"
#define CONFIG_CCU_INIT_BOOTARGS "setenv bootargs console=${console} earlyprintk " 	\
												 "mtdparts=${mtdparts} " 				\
												 "rootfstype=ubifs ubi.mtd=${ubi.tmd} "\
												 "root=ubi0:rootfs"
#define CONFIG_CCU_BOOTCMD		 "run check_1; run init_os; run init_bootargs;" 		\
								 "nand read 0x21000000 ${dtb_os} 0x80000;" 				\
								 "nand read 0x22000000 ${kernel_os} 0x780000;" 			\
								 "bootz 0x22000000 - 0x21000000"



#define CONFIG_CCU_CCU800_ENV_EX	\
								"console="		CCU800_CONSOLE			"\0" \
								"switch_os1="	CCU800_SWITCH_OS1		"\0" \
								"switch_os="	CCU800_SWITCH_OS		"\0" \
								"check_3="		CONFIG_CCU_CHECK_3		"\0" \
								"check_2="		CONFIG_CCU_CHECK_2		"\0" \
								"check_1="		CONFIG_CCU_CHECK_1		"\0" \
								"os1_init="		CONFIG_CCU_OS1_INIT		"\0" \
								"os2_mtdparts="	CONFIG_CCU_OS2_MTDPARTS	"\0" \
								"os2_dtb="		CONFIG_CCU_OS2_DTB		"\0" \
								"os2_kernel="	CONFIG_CCU_OS2_KERNEL	"\0" \
								"os2_rootfs="	CONFIG_CCU_OS2_ROOTFS	"\0" \
								"os2_ubi.tmd="	CONFIG_CCU_OS2_UBITMD	"\0" \
								"os2_init="		CONFIG_CCU_OS2_INIT		"\0" \
								"init_os="		CONFIG_CCU_INIT_OS		"\0" \
								"init_bootargs=" CONFIG_CCU_INIT_BOOTARGS	"\0"
		

//#endif

#ifdef DEFAULT_ENV_INSTANCE_EMBEDDED
env_t environment __PPCENV__ = {
	ENV_CRC,	/* CRC Sum */
#ifdef CONFIG_SYS_REDUNDAND_ENVIRONMENT
	1,		/* Flags: valid */
#endif
	{
#elif defined(DEFAULT_ENV_INSTANCE_STATIC)
static char default_environment[] = {
#else
const uchar default_environment[] = {
#endif
#ifdef	CONFIG_ENV_CALLBACK_LIST_DEFAULT
	ENV_CALLBACK_VAR "=" CONFIG_ENV_CALLBACK_LIST_DEFAULT "\0"
#endif
#ifdef	CONFIG_ENV_FLAGS_LIST_DEFAULT
	ENV_FLAGS_VAR "=" CONFIG_ENV_FLAGS_LIST_DEFAULT "\0"
#endif
//#ifdef	CONFIG_BOOTARGS
//	"bootargs="	CONFIG_BOOTARGS			"\0"
//#endif
	"bootcmd="	CONFIG_CCU_BOOTCMD		"\0"
//#ifdef	CONFIG_BOOTCOMMAND
//	"bootcmd="	CONFIG_BOOTCOMMAND		"\0"
//#endif
#ifdef	CONFIG_RAMBOOTCOMMAND
	"ramboot="	CONFIG_RAMBOOTCOMMAND		"\0"
#endif
#ifdef	CONFIG_NFSBOOTCOMMAND
	"nfsboot="	CONFIG_NFSBOOTCOMMAND		"\0"
#endif
#if defined(CONFIG_BOOTDELAY) && (CONFIG_BOOTDELAY >= 0)
	"bootdelay="	__stringify(CONFIG_BOOTDELAY)	"\0"
#endif
#if defined(CONFIG_BAUDRATE) && (CONFIG_BAUDRATE >= 0)
	"baudrate="	__stringify(CONFIG_BAUDRATE)	"\0"
#endif
#ifdef	CONFIG_LOADS_ECHO
	"loads_echo="	__stringify(CONFIG_LOADS_ECHO)	"\0"
#endif
#ifdef	CONFIG_ETHPRIME
	"ethprime="	CONFIG_ETHPRIME			"\0"
#endif
#ifdef	CONFIG_IPADDR
	"ipaddr="	__stringify(CONFIG_IPADDR)	"\0"
#endif
#ifdef	CONFIG_SERVERIP
	"serverip="	__stringify(CONFIG_SERVERIP)	"\0"
#endif
#ifdef	CONFIG_SYS_AUTOLOAD
	"autoload="	CONFIG_SYS_AUTOLOAD		"\0"
#endif
#ifdef	CONFIG_PREBOOT
	"preboot="	CONFIG_PREBOOT			"\0"
#endif
#ifdef	CONFIG_ROOTPATH
	"rootpath="	CONFIG_ROOTPATH			"\0"
#endif
#ifdef	CONFIG_GATEWAYIP
	"gatewayip="	__stringify(CONFIG_GATEWAYIP)	"\0"
#endif
#ifdef	CONFIG_NETMASK
	"netmask="	__stringify(CONFIG_NETMASK)	"\0"
#endif
#ifdef	CONFIG_HOSTNAME
	"hostname="	__stringify(CONFIG_HOSTNAME)	"\0"
#endif
#ifdef	CONFIG_BOOTFILE
	"bootfile="	CONFIG_BOOTFILE			"\0"
#endif
#ifdef	CONFIG_LOADADDR
	"loadaddr="	__stringify(CONFIG_LOADADDR)	"\0"
#endif
#ifdef	CONFIG_CLOCKS_IN_MHZ
	"clocks_in_mhz=1\0"
#endif
#if defined(CONFIG_PCI_BOOTDELAY) && (CONFIG_PCI_BOOTDELAY > 0)
	"pcidelay="	__stringify(CONFIG_PCI_BOOTDELAY)"\0"
#endif
#ifdef	CONFIG_ENV_VARS_UBOOT_CONFIG
	"arch="		CONFIG_SYS_ARCH			"\0"
#ifdef CONFIG_SYS_CPU
	"cpu="		CONFIG_SYS_CPU			"\0"
#endif
#ifdef CONFIG_SYS_BOARD
	"board="	CONFIG_SYS_BOARD		"\0"
	"board_name="	CONFIG_SYS_BOARD		"\0"
#endif
#ifdef CONFIG_SYS_VENDOR
	"vendor="	CONFIG_SYS_VENDOR		"\0"
#endif
#ifdef CONFIG_SYS_SOC
	"soc="		CONFIG_SYS_SOC			"\0"
#endif
//#ifdef CONFIG_CCU_CCU800_ENV_EX
	CONFIG_CCU_CCU800_ENV_EX
//#endif 
#endif
#ifdef	CONFIG_EXTRA_ENV_SETTINGS
	CONFIG_EXTRA_ENV_SETTINGS
#endif
	"\0"
#ifdef DEFAULT_ENV_INSTANCE_EMBEDDED
	}
#endif
};
