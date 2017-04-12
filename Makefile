#///////////////////////////////////////////////////////////////////////////////
#  Copyright (c) 2017 Clemson University.
# 
#  This file was originally written by Bradley S. Meyer.
# 
#  This is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
# 
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this software; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
#  USA
# 
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#//!
#//! \file
#//! \brief A makefile to generate replace_partf code.
#//!
#///////////////////////////////////////////////////////////////////////////////

ifndef NUCNET_TARGET
NUCNET_TARGET = ../..
endif

NNT_DIR = $(NUCNET_TARGET)/nnt
BUILD_DIR = $(NUCNET_TARGET)/build

#///////////////////////////////////////////////////////////////////////////////
# End of lines to be edited.
#///////////////////////////////////////////////////////////////////////////////

include $(BUILD_DIR)/Makefile

VPATH = $(BUILD_DIR):$(NNT_DIR)

#===============================================================================
# Objects.
#===============================================================================

REPF_OBJ = $(WN_OBJ)         \
           $(NNT_OBJ)	     \

#===============================================================================
# Executables.
#===============================================================================

REPF_EXEC = replace_partf       \
            print_partf_at_t9   \

$(REPF_EXEC): $(REPF_OBJ)
	$(CC) $(REPF_OBJ) -o $(BINDIR)/$@ $@.cpp $(CLIBS)

.PHONY all_replace_partf: $(REPF_EXEC) 

#===============================================================================
# Clean up. 
#===============================================================================

.PHONY: clean_replace_partf cleanall_replace_partf

clean_replace_partf:
	rm -f $(REPF_OBJ)

cleanall_replace_partf: clean_replace_partf
	rm -f $(BINDIR)/$(REPF_EXEC) $(BINDIR)/$(REPF_EXEC).exe
