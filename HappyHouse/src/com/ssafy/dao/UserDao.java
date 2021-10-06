package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.UserDto;

public interface UserDao {
	void register(UserDto userDto);

	UserDto searchById(String userId);

	List<UserDto> searchAll();

	void updateUserInfo(UserDto userDto);

	void deleteUser(String userId);
}
