/*
 * =================================================================
 *
 *
 *	Description:  samsung display panel file
 *
 *	Author: jb09.kim
 *	Company:  Samsung Electronics
 *
 * ================================================================
 */
/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012, Samsung Electronics. All rights reserved.

*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/
#include "ss_dsi_panel_PBA_BOOTING_fhd.h"

static void samsung_pba_config(struct samsung_display_driver_data *vdd, void *arg)
{
	struct sde_mdss_cfg *sde_cfg = (struct sde_mdss_cfg *)arg;

	if (!IS_ERR_OR_NULL(sde_cfg)) {
		LCD_INFO("Disable source split\n");
		sde_cfg->has_src_split = false;
	}
}

static char ss_panel_revision(struct samsung_display_driver_data *vdd)
{
    // Hardcode the panel revision to 'A'
    vdd->panel_revision = 0;  // 'A' corresponds to 0

    LCD_INFO("panel_revision = %c %d \n", vdd->panel_revision + 'A', vdd->panel_revision);

    return (vdd->panel_revision + 'A');
}

static int dummy_poc_write(struct samsung_display_driver_data *vdd,
                           u8 *data, u32 write_pos, u32 write_size)
{
        pr_info("Dummy POC write: pos=0x%x size=%u\n", write_pos, write_size);
        return 0;
}

static void samsung_panel_init(struct samsung_display_driver_data *vdd)
{
	LCD_INFO("%s\n", ss_get_panel_name(vdd));

	vdd->support_mdnie_lite = false;
	vdd->panel_func.samsung_panel_revision = ss_panel_revision;
	vdd->dtsi_data.tft_common_support = true;
	vdd->panel_func.samsung_pba_config = samsung_pba_config;
	vdd->poc_driver.poc_write = dummy_poc_write;
}

static int __init samsung_panel_initialize(void)
{
	struct samsung_display_driver_data *vdd;
	enum ss_display_ndx ndx;
	char panel_string[] = "ss_dsi_panel_PBA_BOOTING_FHD";
	char panel_name[MAX_CMDLINE_PARAM_LEN];
	char panel_secondary_name[MAX_CMDLINE_PARAM_LEN];

	ss_get_primary_panel_name_cmdline(panel_name);
	ss_get_secondary_panel_name_cmdline(panel_secondary_name);

	/* TODO: use component_bind with panel_func
	 * and match by panel_string, instead.
	 */
	if (!strncmp(panel_string, panel_name, strlen(panel_string)))
		ndx = PRIMARY_DISPLAY_NDX;
	else if (!strncmp(panel_string, panel_secondary_name,
				strlen(panel_string)))
		ndx = SECONDARY_DISPLAY_NDX;
	else
		return 0;

	vdd = &vdd_data[ndx];
	vdd->panel_func.samsung_panel_init = samsung_panel_init;

	return 0;


}
early_initcall(samsung_panel_initialize);
