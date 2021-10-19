package service;

import com.ssafy.dto.UserDto;

public interface UserService {

	int usernameCheck(String username) throws Exception;

	void register(UserDto UserDto) throws Exception;

	UserDto login(String username, String password) throws Exception;

	UserDto getUser(String username) throws Exception;

	void updateUser(UserDto UserDto) throws Exception;

	void deleteUser(String username) throws Exception;

}
