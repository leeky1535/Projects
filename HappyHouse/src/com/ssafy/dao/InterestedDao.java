package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.InterestedDto;

public interface InterestedDao {
	void register(String userid, String dong);

	void delete(String userId, String dong);

	List<InterestedDto> searchById(String userId);
}
