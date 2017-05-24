#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x53a8e63d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5d41c87c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xe47270b7, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0xa67caa35, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6b13d642, __VMLINUX_SYMBOL_STR(__dynamic_dev_dbg) },
	{ 0x45a5f288, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x69be0d3, __VMLINUX_SYMBOL_STR(pci_intx_mask_supported) },
	{ 0x9a894084, __VMLINUX_SYMBOL_STR(dev_notice) },
	{ 0x962f7f08, __VMLINUX_SYMBOL_STR(__uio_register_device) },
	{ 0x9cf067da, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0xc80e8770, __VMLINUX_SYMBOL_STR(pci_enable_msix) },
	{ 0xcfc62f27, __VMLINUX_SYMBOL_STR(xen_start_info) },
	{ 0x731dba7a, __VMLINUX_SYMBOL_STR(xen_domain_type) },
	{ 0xef617880, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0xdbedeefe, __VMLINUX_SYMBOL_STR(dma_set_mask) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0xa5203407, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0xaa1f6354, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x6153b4b9, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0x215b60b8, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x83b32430, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xb6acb342, __VMLINUX_SYMBOL_STR(pci_check_and_mask_intx) },
	{ 0xb1c316af, __VMLINUX_SYMBOL_STR(pci_intx) },
	{ 0x5113ca0e, __VMLINUX_SYMBOL_STR(pci_cfg_access_unlock) },
	{ 0xe1ccee8b, __VMLINUX_SYMBOL_STR(pci_cfg_access_lock) },
	{ 0xf02109f5, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xc715d9e0, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0xefaba24b, __VMLINUX_SYMBOL_STR(pci_disable_msix) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xc2f1cbce, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0xb31c665a, __VMLINUX_SYMBOL_STR(uio_unregister_device) },
	{ 0x18851c14, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xc6b79d76, __VMLINUX_SYMBOL_STR(pci_bus_type) },
	{ 0x4b97c8c3, __VMLINUX_SYMBOL_STR(pci_enable_sriov) },
	{ 0xe8e4088f, __VMLINUX_SYMBOL_STR(pci_num_vf) },
	{ 0xfb374c50, __VMLINUX_SYMBOL_STR(pci_disable_sriov) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=uio";


MODULE_INFO(srcversion, "47AE41AD0F20FAA13A85D76");
