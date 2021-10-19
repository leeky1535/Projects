package service;

import java.util.List;

import com.ssafy.dao.AnnouncementDao;
import com.ssafy.daoImpl.AnnouncementDaoImpl;
import com.ssafy.dto.AnnouncementDto;

public class AnnouncementServiceImpl implements AnnouncementService {

	private static AnnouncementService annService = new AnnouncementServiceImpl();
	
	private AnnouncementDao annDao;
	
	private AnnouncementServiceImpl() {
		annDao  = AnnouncementDaoImpl.getAnnDao();
	}
	
	public static AnnouncementService getAnnService() {
		return annService;
	}

	@Override
	public void writeAnnouncement(AnnouncementDto annDto) throws Exception {
		annDao.writeAnnouncement(annDto);
	}

	@Override
	public List<AnnouncementDto> listAnnouncement(String word) throws Exception {
		word = word == null ? "" : word.trim();
		return annDao.listAnnouncement(word);
	}

	@Override
	public AnnouncementDto getAnnouncement(int annNo) throws Exception {
		return annDao.getAnnouncement(annNo);
	}

	@Override
	public void updateAnnouncement(AnnouncementDto annDto) throws Exception {
		annDao.updateAnnouncement(annDto);

	}

	@Override
	public void deleteAnnouncement(int annNo) throws Exception {
		annDao.deleteAnnouncement(annNo);

	}
}
