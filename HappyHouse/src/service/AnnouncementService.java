package service;

import java.util.List;

import com.ssafy.dto.AnnouncementDto;

public interface AnnouncementService {
	void writeAnnouncement(AnnouncementDto annDto) throws Exception;

	List<AnnouncementDto> listAnnouncement(String word) throws Exception;

	AnnouncementDto getAnnouncement(int annNo) throws Exception;

	void updateAnnouncement(AnnouncementDto annDto) throws Exception;

	void deleteAnnouncement(int annNo) throws Exception;
	
}
