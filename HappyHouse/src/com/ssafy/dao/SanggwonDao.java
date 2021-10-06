package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.SanggwonDto;

public interface SanggwonDao {
	List<SanggwonDto> searchAll();

	List<SanggwonDto> searchByDongcodeCategoryName(String dongCode, String category, String name);
}
